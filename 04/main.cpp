#include <iostream>
#include <vector>
#include <cmath>
#include <iomanip>
#include <algorithm>
#include <string>
#include <utility>
#include "../utils/import_from_file.cpp"

using namespace std;

typedef vector<vector<double>> Matrix;
typedef vector<double> Vector;
typedef vector<int> IndexVector;

void printMatrix(const Matrix& A, const string& title) {
    cout << "\n" << title << ":\n";
    for (size_t i = 0; i < A.size(); i++) {
        for (size_t j = 0; j < A[i].size(); j++) {
            cout << setw(20) << fixed << setprecision(6) << A[i][j];
        }
        cout << "\n";
    }
}

void printVector(const Vector& x, const string& title) {
    cout << "\n" << title << " :\n";
    for (size_t i = 0; i < x.size(); i++) {
        cout << "x" << i << " to " << fixed << setprecision(6) << x[i] << "\n";
    }
}

void printOrder(const IndexVector& v) {
    cout << "\nkol zam:\n";
    for (size_t i = 0; i < v.size(); i++) {
        cout << "kol[" << i << "] = " << v[i] << "\n";
    }
}

bool isValidAugmented(const Matrix& A) {
    if (A.empty()) {
        return false;
    }
    size_t n = A.size();
    for (size_t i = 0; i < n; i++) {
        if (A[i].size() != n + 1) {
            return false;
        }
    }
    return true;
}

bool backSubstitution(const Matrix& A, Vector& x, double eps = 1e-15) {
    int n = static_cast<int>(A.size());
    x.assign(n, 0.0);
    for (int i = n - 1; i >= 0; i--) {
        if (std::abs(A[i][i]) < eps) {
            return false;
        }
        double s = 0.0;
        for (int j = i + 1; j < n; j++) {
            s += A[i][j] * x[j];
        }
        x[i] = (A[i][n] - s) / A[i][i];
    }
    return true;
}

bool gaussianEliminationNoPivoting(Matrix& A, Vector& x, double eps = 1e-15) {
    int n = static_cast<int>(A.size());
    for (int k = 0; k < n - 1; k++) {
        if (std::abs(A[k][k]) < eps) {
            return false;
        }
        for (int i = k + 1; i < n; i++) {
            double factor = A[i][k] / A[k][k];
            for (int j = k; j <= n; j++) {
                A[i][j] -= factor * A[k][j];
            }
        }
    }
    return backSubstitution(A, x, eps);
}

bool gaussianEliminationPartialPivoting(Matrix& A, Vector& x, double eps = 1e-15) {
    int n = static_cast<int>(A.size());
    for (int k = 0; k < n - 1; k++) {
        int maxRow = k;
        for (int i = k + 1; i < n; i++) {
            if (std::abs(A[i][k]) > std::abs(A[maxRow][k])) {
                maxRow = i;
            }
        }
        if (std::abs(A[maxRow][k]) < eps) {
            return false;
        }
        if (maxRow != k) {
            swap(A[k], A[maxRow]);
        }
        for (int i = k + 1; i < n; i++) {
            double factor = A[i][k] / A[k][k];
            for (int j = k; j <= n; j++) {
                A[i][j] -= factor * A[k][j];
            }
        }
    }
    return backSubstitution(A, x, eps);
}

bool gaussianEliminationCrout(Matrix& A, Vector& x, IndexVector& colOrder, double eps = 1e-15) {
    int n = static_cast<int>(A.size());
    colOrder.resize(n);
    for (int i = 0; i < n; i++) {
        colOrder[i] = i;
    }
    for (int k = 0; k < n - 1; k++) {
        int maxCol = k;
        for (int j = k + 1; j < n; j++) {
            if (std::abs(A[k][j]) > std::abs(A[k][maxCol])) {
                maxCol = j;
            }
        }
        if (std::abs(A[k][maxCol]) < eps) {
            return false;
        }
        if (maxCol != k) {
            for (int i = 0; i < n; i++) {
                swap(A[i][k], A[i][maxCol]);
            }
            swap(colOrder[k], colOrder[maxCol]);
        }
        for (int i = k + 1; i < n; i++) {
            double factor = A[i][k] / A[k][k];
            for (int j = k; j <= n; j++) {
                A[i][j] -= factor * A[k][j];
            }
        }
    }

    Vector tempX;
    if (!backSubstitution(A, tempX, eps)) {
        return false;
    }

    x.assign(n, 0.0);
    for (int i = 0; i < n; i++) {
        x[colOrder[i]] = tempX[i];
    }
    return true;
}

void runCase(const Matrix& A, const string& name) {
    cout << "\n" << name << ":\n";

    printMatrix(A, "mac roz");

    Matrix A1 = A;
    Vector x1;
    if (gaussianEliminationPartialPivoting(A1, x1)) {
        printMatrix(A1, "po elim forward partial");
        printVector(x1, "Rozwiazanie partial");
    } else {
        cout << "Blad: zero pivot\n";
    }

    Matrix A2 = A;
    Vector x2;
    IndexVector colOrder;
    if (gaussianEliminationCrout(A2, x2, colOrder)) {
        printMatrix(A2, "po crout");
        printOrder(colOrder);
        printVector(x2, "wynik crout");
    } else {
        cout << "Blad: zero pivot\n";
    }

    Matrix A3 = A;
    Vector x3;
    if (gaussianEliminationNoPivoting(A3, x3)) {
        printMatrix(A3, "po elim bez pivot");
        printVector(x3, "sol bez pivot");
    } else {
        cout << "Blad: zero piv\n";
    }
}

vector<pair<string, Matrix>> buildPdfCases() {
    vector<pair<string, Matrix>> cases;

    cases.push_back({
        "zad1",
        {
            {1e-20, 1.0, 1.0},
            {1.0, 1.0, 2.0},
        }
    });

    cases.push_back({
        "zad2",
        {
            {1.0, 1e8, 1e8, 2e8 + 1.0},
            {1.0, 1e8 + 1.0, 1e8, 2e8 + 2.0},
            {1.0, 1e8, 1e8 + 1.0, 2e8 + 2.0},
        }
    });

    cases.push_back({
        "zad3",
        {
            {1.0, 1.0, 1.0, 3.0},
            {1.0, 1.0000001, 1.0, 3.0000001},
            {1.0, 1.0, 1.0000001, 3.0000001},
        }
    });

    cases.push_back({
        "zad4",
        {
            {1.0, 1e10, 1.0, 1e10 + 2.0},
            {1.0, 1e10 + 1.0, 1.0, 1e10 + 3.0},
            {1.0, 1e10, 2.0, 1e10 + 3.0},
        }
    });

    cases.push_back({
        "zad5",
        {
            {1.0, 1.0, 1.0, 1.0, 4.0},
            {1.0, 1.0001, 1.00020001, 1.000300030001, 4.000600040001},
            {1.0, 1.0002, 1.00040004, 1.000600120008, 4.001200160008},
            {1.0, 1.0003, 1.00060009, 1.000900270027, 4.001800360027},
        }
    });

    cases.push_back({
        "zad6",
        {
            {1.0, 1.0, 2.0},
            {1.0, 1.00000000000001, 2.00000000000001},
        }
    });

    return cases;
}

int main() {
    string in;
    if (!(cin >> in)) {
        return 1;
    }

    if (in == "pdf") {
        vector<pair<string, Matrix>> cases = buildPdfCases();
        for (size_t i = 0; i < cases.size(); i++) {
            runCase(cases[i].second, cases[i].first);
        }
        return 0;
    }

    Matrix A;
    try {
        A = utils::read_matrix(in);
    } catch (const exception& e) {
        cerr << "Blad: " << e.what() << "\n";
        return 1;
    }

    if (!isValidAugmented(A)) {
        cerr << "Blad: zly format macierzy\n";
        return 1;
    }

    runCase(A, "z pliku");
    return 0;
}
