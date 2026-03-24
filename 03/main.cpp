#include <cmath>
#include <iostream>
#include <vector>
#include <iomanip>
#include "../utils/import_from_file.cpp"

int main() {
    std::vector<std::vector<double>> X = utils::read_matrix("dane.txt");

    int n = X.size();
    const double EPS = 1e-12;

    for (const auto& row : X) {
        for (double val : row) {
            std::cout << val << " ";
        }
        std::cout << "\n";
    }

    std::cout << "\n\n";

    for (int k = 0; k < n - 1; k++) {

        int maxRow = k;
        for (int i = k + 1; i < n; i++) {
            if (std::abs(X[i][k]) > std::abs(X[maxRow][k])) {
                maxRow = i;
            }
        }

        if (maxRow != k) {
            std::swap(X[k], X[maxRow]);
        }

        if (std::abs(X[k][k]) < EPS) {
            std::cout << "Brak rozwiazania lub nieskonczenie wiele rozwiazan\n";
            return -1;
        }

        for (int i = k + 1; i < n; i++) {
            double m = X[i][k] / X[k][k];
            for (int j = k; j <= n; j++) {
                X[i][j] -= m * X[k][j];
            }
        }
    }

    for (const auto& row : X) {
        for (double val : row) {
            std::cout << val << " ";
        }
        std::cout << "\n";
    }

    std::vector<double> x(n);

    for (int i = n - 1; i >= 0; i--) {
        double sum = 0.0;

        for (int j = i + 1; j < n; j++) {
            sum += X[i][j] * x[j];
        }

        if (std::abs(X[i][i]) < EPS) {
            std::cout << "Dzielenie przez 0 w back substitution\n";
            return -1;
        }

        x[i] = (X[i][n] - sum) / X[i][i];
    }

    std::cout << "\nRozwiazania ukladu:\n";
    std::cout << std::fixed << std::setprecision(6);

    for (int i = 0; i < n; i++) {
        std::cout << "x" << i << " = " << x[i] << "\n";
    }

    return 0;
}