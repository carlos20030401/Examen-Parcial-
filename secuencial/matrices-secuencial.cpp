
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <fstream>

using namespace std;
using namespace chrono;

class Matriz {
private:
    int size;
    vector<vector<int>> datos;

public:
    Matriz(int n) : size(n), datos(n, vector<int>(n)) {
        generarAleatorios();
    }

    void generarAleatorios() {
        for (int i = 0; i < size; ++i)
            for (int j = 0; j < size; ++j)
                datos[i][j] = rand() % 10; // Numeros randoms o aleatorios de 0 a 9
    }

    int sumarElementos() {
        int suma = 0;
        for (int i = 0; i < size; ++i)
            for (int j = 0; j < size; ++j)
                suma += datos[i][j];
        return suma;
    }
};
int main() {
    srand(time(0)); // Aca es el brote para inicialisar con los aleatorios
    int N;
    cout << "Ingrese el tamano de la matriz cuadrada: ";
    cin >> N;