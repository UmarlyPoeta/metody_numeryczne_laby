#include "../utils/import_from_file.cpp"
#include <iostream>
#include <vector>
#include <iomanip>

using namespace std;

double interpolate(vector<double> X, vector<double> f_X, double xi, int N) {
    double result = 0;

    for (int i = 0; i < N; i++) {
        double term = f_X[i];

        for (int j = 0; j < N; j++) {
            if (j != i) {
                term = term * (xi - X[j]) / (X[i] - X[j]);
            }
        }

        result += term;
    }

    return result;
}

int main() {
    std::pair<std::pair<int,int>, std::vector<std::pair<double, double>>> data = 
        utils::read_typed_file("dane.txt");

    int N = data.first.first;

    vector<double> X;
    vector<double> f_X;
    
    for (auto d : data.second) {
        X.push_back(d.first);
        f_X.push_back(d.second);
    }

    cout << "=== Lagrange Interpolation ===" << endl;
    cout << "Number of nodes: " << N << endl;
    cout << "\nInterpolation data:" << endl;
    for (int i = 0; i < N; i++) {
        cout << "x[" << i << "] = " << fixed << setprecision(4) << X[i] 
             << ", f(x[" << i << "]) = " << f_X[i] << endl;
    }

    double xi;
    cout << "\nEnter point x: ";
    cin >> xi;

    cout << "f(" << xi << ") = " << fixed << setprecision(6) 
         << interpolate(X, f_X, xi, N) << endl;

    return 0;
}
