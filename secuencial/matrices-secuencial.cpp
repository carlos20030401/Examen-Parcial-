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
        generarAleatorios(