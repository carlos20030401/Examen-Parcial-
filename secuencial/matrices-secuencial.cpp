#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <fstream>
#include <direct.h>  // Para crear directorios 

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
                datos[i][j] = rand() % 10; // Números aleatorios de 0 a 9
    }

    int sumarElementos() {
        int suma = 0;
        for (int i = 0; i < size; ++i)
            for (int j = 0; j < size; ++j)
                suma += datos[i][j];
        return suma;
    }
};

bool crearDirectorio(const string& path) {
    // Para crear un directorio, en Windows usamos _mkdir
    return _mkdir(path.c_str()) == 0;
}

int main() {
    srand(time(0)); // Semilla para números aleatorios
    int N;
    cout << "Ingrese el tamaño máximo N de la matriz cuadrada: ";
    cin >> N;

    // Asegúrate de que la carpeta 'secuencial' exista
    if (!crearDirectorio("secuencial")) {
        cerr << "No se pudo crear el directorio 'secuencial'.\n";
        return 1;
    }

    ofstream archivo("secuencial/secuencial.csv");
    if (!archivo.is_open()) {
        cerr << "No se pudo abrir secuencial.csv para escritura.\n";
        return 1;
    }

    // Escribe los encabezados en el archivo CSV
    archivo << "Tamaño de matriz,Tiempo de ejecución (ms)\n";

    for (int n = 1; n <= N; ++n) {
        auto inicio = high_resolution_clock::now();
        Matriz matriz(n);
        int suma = matriz.sumarElementos();
        auto fin = high_resolution_clock::now();

        duration<double, milli> duracion = fin - inicio;

        archivo << n << "," << duracion.count() << endl;
        cout << "Matriz " << n << "x" << n << ": suma = " << suma
              << ", tiempo = " << duracion.count() << " ms\n";
    }

    archivo.close();
    return 0;
}
