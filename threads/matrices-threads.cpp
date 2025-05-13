#include <iostream>
#include <vector>
#include <thread>
#include <chrono>
#include <random>

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
    cout << "Ingrese el valor de N: ";
    cin >> N;
