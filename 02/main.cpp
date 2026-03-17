#include <iostream>
#include <cmath>
#include "../utils/import_from_file.cpp"
#include <vector>


int main() {
    std::pair<std::pair<double, double>, std::vector<std::pair<double, double>>> data = utils::read_typed_file("dane.txt");


    std::vector<double> X;
    std::vector<double> y;


    for (auto x : data.second) {
        X.push_back(x.first);
    }
    for (auto f_x : data.second) {
        y.push_back(f_x.second);
    }

    double p_o = 1;
    double b_o = y[0];

    double x;

    std::cin >> x;

    std::vector<double> b = y;
    for (int j = 1; j < X.size(); ++j) {
        for (int i = X.size() - 1; i >= j; --i) {
            b[i] = (b[i] - b[i - 1]) / (X[i] - X[i - j]);
        }
    }

    double W_n = b[0];
    double p = 1.0;
    for (int k = 1; k < X.size(); ++k) {
        p *= (x - X[k - 1]);
        W_n += b[k] * p;
    }

    std::cout << W_n << std::endl;
    return 0;
}