#include <iostream>
#include <vector>
#include <thread>
#include <chrono>
#include <random>

using namespace std;
mutex mtx;  // para proteger el acceso a la suma global
class Matriz {
private:
    int N;
    vector<vector<int>> datos;
public:
    Matriz(int n) : N(n), datos(n, vector<int>()) {
        random_device rd;
        mt19937 gen(rd());
        uniform_int_distribution<> dis(0, 9);
        for (int i = 0; i < N ++i)