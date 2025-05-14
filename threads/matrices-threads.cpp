#include <iostream>
#include <vector>
#include <thread>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <atomic>
#include <chrono>
#include <random>
#include <fstream>

using namespace std;

// ========== ThreadPool integrado ==========

class ThreadPool {
private:
    vector<thread> workers;
    queue<function<void()>> tasks;
    mutex queue_mutex;
    condition_variable condition;
    atomic<bool> stop;

public:
    ThreadPool(size_t threads) : stop(false) {
        for (size_t i = 0; i < threads; ++i) {
            workers.emplace_back([this] {
                while (true) {
                    function<void()> task;
                    {
                        unique_lock<mutex> lock(this->queue_mutex);
                        this->condition.wait(lock, [this] {
                            return this->stop || !this->tasks.empty();
                        });
                        if (this->stop && this->tasks.empty())
                            return;
                        task = move(this->tasks.front());
                        this->tasks.pop();
                    }
                    task();
                }
            });
        }
    }

    void enqueue(function<void()> f) {
        {
            unique_lock<mutex> lock(queue_mutex);
            tasks.push(move(f));
        }
        condition.notify_one();
    }

    ~ThreadPool() {
        stop = true;
        condition.notify_all();
        for (thread &worker : workers)
            worker.join();
    }
};

// ========== Clase Matriz ==========

class Matriz {
private:
    int N;
    vector<vector<int>> datos;
public:
    Matriz(int n) : N(n), datos(n, vector<int>(n)) {
        random_device rd;
        mt19937 gen(rd());
        uniform_int_distribution<> dis(0, 9);
        for (int i = 0; i < N; ++i)
            for (int j = 0; j < N; ++j)
                datos[i][j] = dis(gen);
    }

    int getValor(int i, int j) const {
        return datos[i][j];
    }

    int getSize() const {
        return N;
    }
};

// ========== Variable global protegida ==========

mutex mtx;

int main() {
    int N;
    cout << "Ingrese el tamaño máximo N de la matriz cuadrada: ";
    cin >> N;

    ofstream archivo("threads_pool.csv");
    archivo << "Tamaño de matriz,Tiempo de ejecución (ms),Suma total\n";

    int maxThreads = thread::hardware_concurrency();
    maxThreads = maxThreads > 0 ? maxThreads : 4;

    ThreadPool pool(maxThreads);

    for (int n = 1; n <= N; ++n) {
        Matriz matriz(n);
        long long sumaTotal = 0;

        int numThreads = min(maxThreads, n);
        int filasPorHilo = n / numThreads;
        int resto = n % numThreads;

        atomic<int> tareasPendientes(0);
        auto inicio = chrono::high_resolution_clock::now();

        int inicioFila = 0;
        for (int i = 0; i < numThreads; ++i) {
            int finFila = inicioFila + filasPorHilo + (i < resto ? 1 : 0);
            tareasPendientes++;
            pool.enqueue([&, inicioFila, finFila]() {
                long long localSuma = 0;
                for (int i = inicioFila; i < finFila; ++i)
                    for (int j = 0; j < matriz.getSize(); ++j)
                        localSuma += matriz.getValor(i, j);
                {
                    lock_guard<mutex> lock(mtx);
                    sumaTotal += localSuma;
                }
                tareasPendientes--;
            });
            inicioFila = finFila;
        }

        while (tareasPendientes > 0) {
            this_thread::sleep_for(chrono::milliseconds(1));
        }

        auto fin = chrono::high_resolution_clock::now();
        chrono::duration<double, milli> duracion = fin - inicio;

        archivo << n << "," << duracion.count() << "," << sumaTotal << "\n";
        cout << "Matriz " << n << "x" << n << ": suma = " << sumaTotal
             << ", tiempo = " << static_cast<int>(duracion.count()) << " ms" << endl;
    }

    archivo.close();
    return 0;
}


