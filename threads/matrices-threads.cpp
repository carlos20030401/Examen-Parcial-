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