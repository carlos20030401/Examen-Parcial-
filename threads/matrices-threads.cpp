#include <iostream>
#include <vector>
#include <thread>
#include <chrono>
#include <fstream>
#include <mutex>
#include <random>

using namespace std;
mutex mtx;  // para proteger el acceso a la suma global

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

void sumarFila(const Matriz& matriz, int filaInicio, int filaFin, long long& sumaParcial) {
    long long localSuma = 0;
    for (int i = filaInicio; i < filaFin; ++i) {
        for (int j = 0; j < matriz.getSize(); ++j) {
            localSuma += matriz.getValor(i, j);
        }
    }
    lock_guard<mutex> lock(mtx);
    sumaParcial += localSuma;
}

int main() {
    int N;
    cout << "Ingrese el tamano maximo N de la matriz cuadrada: ";
    cin >> N;

    ofstream archivo("threads.csv");
    archivo << "Tamaño de matriz,Tiempo de ejecución (ms),Suma total\n";

    for (int n = 1; n <= N; ++n) {
        Matriz matriz(n);
        long long sumaTotal = 0;

        int numThreads = thread::hardware_concurrency();
        numThreads = numThreads > n ? n : numThreads;
        numThreads = numThreads > 0 ? numThreads : 4;

        vector<thread> threads;
        int filasPorHilo = n / numThreads;
        int resto = n % numThreads;

        auto inicio = chrono::high_resolution_clock::now();

        int inicioFila = 0;
        for (int i = 0; i < numThreads; ++i) {
            int finFila = inicioFila + filasPorHilo + (i < resto ? 1 : 0);
            threads.emplace_back(sumarFila, ref(matriz), inicioFila, finFila, ref(sumaTotal));
            inicioFila = finFila;
        }

        for (auto& t : threads) {
            t.join();
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


