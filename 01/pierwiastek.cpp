#include <iostream>
#include <vector>
#include <iomanip>
#include <cmath>

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
    vector<double> X;
    vector<double> f_X;
    double xi_c = 50.0; 

    X.push_back(27.0);
    X.push_back(64.0);
    X.push_back(125.0);
    X.push_back(216.0);

    for (int i = 0; i < 4; i++) {
        f_X.push_back(pow(X[i], 1.0 / 3.0));  
    }

    int N = 4;

    cout << fixed << setprecision(6) << interpolate(X, f_X, xi_c, N) << endl;


}