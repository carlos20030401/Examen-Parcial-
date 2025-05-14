#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>

using namespace std;

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

    int getValor(int i, int j) const {
        return datos[i][j];
    }

    void imprimir() const {
        for (int i = 0; i < size; ++i) {
            cout << "[ ";
            for (int j = 0; j < size; ++j)
                cout << datos[i][j] << " ";
            cout << "]\n";
        }
    }

    int sumarElementos() {
        int suma = 0;
        for (int i = 0; i < size; ++i)
            for (int j = 0; j < size; ++j)
                suma += datos[i][j];
        return suma;
    }
};


/*int main() {
    srand(time(0)); // Semilla para números aleatorios
    int N;
    cout << "Ingrese el tamano maximo N de la matriz cuadrada: ";
    cin >> N;

    ofstream archivo("secuencial.csv");
    if (!archivo.is_open()) {
        cerr << "No se pudo abrir secuencial.csv para escritura.\n";
        return 1;
    }

    // Escribe los encabezados en el archivo CSV
    archivo << "Tamano de matriz,Tiempo de ejecucion (ms)\n";

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
}*/

int main() {
    srand(0); // Semilla fija para generar la misma matriz

    for (int n = 1; n <= 10; ++n) {
        Matriz matriz(n);
        cout << "\nMatriz " << n << "x" << n << ":\n";
        matriz.imprimir();
        int suma = matriz.sumarElementos();
        cout << "Secuencial: S = " << suma << "\n";
    }

    return 0;
}
	
