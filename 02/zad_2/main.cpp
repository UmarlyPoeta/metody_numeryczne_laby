#include <iostream>
#include <cmath>
#include "../../utils/import_from_file.cpp"
#include <vector>
#include <fstream>


int main() {
    std::ofstream file("wspolczynniki.txt");
    std::vector<double> X;
    std::vector<double> y;
    double l_wez = 5.0f;

    double odl = 2.0f / l_wez;

    for (int i = 0; i < l_wez; i++) {
        double arg = -1.0 + i * odl;
        X.push_back(arg);
        double war = 1.0f / (1 + 25.0 * pow(arg, 2));
        y.push_back(war);
    }

    std::vector<double> b = y;

    for (int j = 1; j < X.size(); ++j) {
        for (int i = X.size() - 1; i >= j; --i) {
            b[i] = (b[i] - b[i - 1]) / (X[i] - X[i - j]);
        }
    }

    //double W_n = b[0];
    //double p = 1.0;
    /*for (int k = 1; k < X.size(); ++k) {
        p *= (x - X[k - 1]);
        W_n += b[k] * p;
    }

    for (int i = 0; i < b.size(); i++) {
        std::cout <<"wsp. wiel. nr. " << i + 1 << ": " << b[i] << std::endl;
    }

    std::cout << W_n << std::endl;
    */

    for (int i = 0; i < b.size(); i++) {
        file << b[i] << "\n";
    }

    return 0;
}