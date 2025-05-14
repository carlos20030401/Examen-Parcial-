#include <iostream>
#include <vector>
#include <thread>
#include <chrono>
#include <fstream>
#include <mutex>
#include <cstdlib>
#include <queue>
#include <condition_variable>
#include <functional>
#include <atomic>
#include <future>

using namespace std;

// ========== ThreadPool ==========

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

    template<class F>
    auto enqueue(F&& f) -> future<decltype(f())> {
        auto task = make_shared<packaged_task<decltype(f())()>>(forward<F>(f));
        future<decltype(f())> res = task->get_future();
        {
            unique_lock<mutex> lock(queue_mutex);
            tasks.emplace([task]() { (*task)(); });
        }
        condition.notify_one();
        return res;
    }

    ~ThreadPool() {
        stop = true;
        condition.notify_all();
        for (thread &worker : workers)
            worker.join();
    }
};

// ========== Matriz y función de suma ==========

mutex mtx;  // proteger suma global

class Matriz {
private:
    int N;
    vector<vector<int>> datos;
public:
    Matriz(int n) : N(n), datos(n, vector<int>(n)) {
        for (int i = 0; i < N; ++i)
            for (int j = 0; j < N; ++j)
                datos[i][j] = rand() % 10;
    }

    int getValor(int i, int j) const {
        return datos[i][j];
    }

    int getSize() const {
        return N;
    }

    void imprimir() const {
        for (int i = 0; i < N; ++i) {
            cout << "[ ";
            for (int j = 0; j < N; ++j)
                cout << datos[i][j] << " ";
            cout << "]\n";
        }
    }
};

pair<long long, string> sumarFila(const Matriz& matriz, int filaInicio, int filaFin) {
    long long localSuma = 0;
    string texto = "t" + to_string(filaInicio + 1) + " = ";
    for (int i = filaInicio; i < filaFin; ++i) {
        for (int j = 0; j < matriz.getSize(); ++j) {
            localSuma += matriz.getValor(i, j);
            texto += to_string(matriz.getValor(i, j)) + " + ";
        }
    }
    if (!texto.empty()) texto = texto.substr(0, texto.size() - 3); // quitar último +
    texto += " = " + to_string(localSuma);
    return {localSuma, texto};
}

// ========== main ==========

int main() {
    srand(0); // para que la matriz sea siempre igual

    for (int n = 1; n <= 10; ++n) {
        Matriz matriz(n);
        matriz.imprimir();

        long long sumaTotal = 0;

        ThreadPool pool(n);  // un hilo por fila
        vector<future<pair<long long, string>>> resultados;

        for (int i = 0; i < n; ++i) {
            resultados.emplace_back(pool.enqueue([&matriz, i] {
                return sumarFila(matriz, i, i + 1);
            }));
        }

        for (auto& f : resultados) {
            auto [suma, detalle] = f.get();
            sumaTotal += suma;
            cout << detalle << endl;
        }

        cout << "S = " << sumaTotal << "\n\n";
    }

    return 0;
}

