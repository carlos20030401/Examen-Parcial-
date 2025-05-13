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
                datos[i][j] = rand() % 10; // Numeros randoms o aleatorios de 0 a 9
    }
