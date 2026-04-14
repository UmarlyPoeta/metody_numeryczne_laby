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

void printMatrix(const Matrix& A, const string& title, int precision = 6, bool useScientific = false) {
    ios::fmtflags oldFlags = cout.flags();
    streamsize oldPrecision = cout.precision();

    cout << "\n" << title << ":\n";
    if (useScientific) {
        cout << scientific;
    } else {
        cout << fixed;
    }
    cout << setprecision(precision);

    for (size_t i = 0; i < A.size(); i++) {
        for (size_t j = 0; j < A[i].size(); j++) {
            cout << setw(24) << A[i][j];
        }
        cout << "\n";
    }

    cout.flags(oldFlags);
    cout.precision(oldPrecision);
}

void printVector(const Vector& x, const string& title, int precision = 6, bool useScientific = false) {
    ios::fmtflags oldFlags = cout.flags();
    streamsize oldPrecision = cout.precision();

    if (useScientific) {
        cout << scientific;
    } else {
        cout << fixed;
    }
    cout << setprecision(precision);

    cout << "\n" << title << " :\n";
    for (size_t i = 0; i < x.size(); i++) {
        cout << "x" << i << " = " << x[i] << "\n";
    }

    cout.flags(oldFlags);
    cout.precision(oldPrecision);
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

// powyzej z czata helper func

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

bool luDoolittle(const Matrix& A, Matrix& L, Matrix& U, double eps = 1e-15) {
    int n = static_cast<int>(A.size());
    if (n == 0 || static_cast<int>(A[0].size()) != n) {
        return false;
    }

    L.assign(n, Vector(n, 0.0));
    U.assign(n, Vector(n, 0.0));

    for (int i = 0; i < n; i++) {
        L[i][i] = 1.0;
    }

    for (int k = 0; k < n; k++) {

        for (int j = k; j < n; j++) {
            double sum = 0.0;
            for (int s = 0; s < k; s++) {
                sum += L[k][s] * U[s][j];
            }
            U[k][j] = A[k][j] - sum;
        }

        if (std::abs(U[k][k]) < eps) {
            return false; 
        }

        for (int i = k + 1; i < n; i++) {
            double sum = 0.0;
            for (int s = 0; s < k; s++) {
                sum += L[i][s] * U[s][k];
            }
            L[i][k] = (A[i][k] - sum) / U[k][k];
        }
    }

    return true;
}
bool forwardSubstitution(const Matrix& L, const Vector& b, Vector& y, double eps = 1e-15) {
    int n = static_cast<int>(L.size());
    if (n == 0 || static_cast<int>(L[0].size()) != n || static_cast<int>(b.size()) != n) {
        return false;
    }
    y.assign(n, 0.0);

    for (int i = 0; i < n; i++) {
        double sum = 0.0;
        for (int j = 0; j < i; j++) {
            sum += L[i][j] * y[j];
        }
        if (std::abs(L[i][i]) < eps) {
            return false;
        }
        y[i] = (b[i] - sum) / L[i][i];
    }
    return true;
}
bool backSubstitutionUpper(const Matrix& U, const Vector& y, Vector& x, double eps = 1e-15) {
    int n = static_cast<int>(U.size());
    if (n == 0 || static_cast<int>(U[0].size()) != n || static_cast<int>(y.size()) != n) {
        return false;
    }
    x.assign(n, 0.0);

    for (int i = n - 1; i >= 0; i--) {
        if (std::abs(U[i][i]) < eps) {
            return false;
        }
        double sum = 0.0;
        for (int j = i + 1; j < n; j++) {
            sum += U[i][j] * x[j];
        }
        x[i] = (y[i] - sum) / U[i][i];
    }
    return true;
}

// z czata to brania macierzy i wektora
bool readSystemFromFile(const string& filename, Matrix& A, Vector& b) {
    Matrix raw = utils::read_matrix(filename);
    if (raw.empty()) {
        cerr << "Blad: pusty plik lub niepoprawne dane: " << filename << "\n";
        return false;
    }

    size_t cols = raw[0].size();
    if (cols == 0) {
        cerr << "Blad: pusta pierwsza linia pliku: " << filename << "\n";
        return false;
    }

    A.clear();
    b.clear();

    for (const auto& row : raw) {
        if (row.size() == cols) {
            A.push_back(row);
        } else if (row.size() == 1) {
            b.push_back(row[0]);
        } else {
            cerr << "Blad: niejednorodny wiersz w pliku (ani macierz, ani wektor): " << filename << "\n";
            return false;
        }
    }

    if (A.empty()) {
        cerr << "Blad: nie wczytano zadnej macierzy A z pliku: " << filename << "\n";
        return false;
    }
    if (A.size() != b.size()) {
        cerr << "Blad: liczba wierszy macierzy A nie zgadza sie z rozmiarem wektora b.\n";
        return false;
    }
    if (A[0].size() != A.size()) {
        cerr << "Blad: macierz A nie jest kwadratowa.\n";
        return false;
    }

    return true;
}


void runScalingExperiment() {
    cout << "zad2\n";
    Matrix A = {
        {1.0, 1.0},
        {1.0, 1.00000000000001}
    };
    Vector b = {2.0, 2.00000000000001};
    Vector x_exact = {1.0, 1.0};

    vector<double> scales = {1.0, 1e14, 1e-14};
    
    
    const double epsExperiment = 1e-40;
    for (double s : scales) {
        Matrix Ascaled = A;
        Vector bscaled = b;
        for (auto& row : Ascaled) {
            for (auto& val : row) {
                val *= s;
            }
        }
        for (auto& val : bscaled) {
            val *= s;
        }

        cout << "skalowanie " << s << "\n";
        printMatrix(Ascaled, " A", 16, true);
        printVector(bscaled, " b", 16, true);

        Matrix L, U;
        if (!luDoolittle(Ascaled, L, U, epsExperiment)) {
            cout << "macierz osobliwa numerycznie\n";
            continue;
        }

        printMatrix(L, " L", 16, true);
        printMatrix(U, " U", 16, true);

        Vector y, x;
        if (!forwardSubstitution(L, bscaled, y, epsExperiment) || !backSubstitutionUpper(U, y, x, epsExperiment)) {
            cout << "nie da sie po LU.\n";
            continue;
        }

        printVector(y, "roz y", 16, true);
        printVector(x, "roz x", 16, true);

        double max_err = 0.0;
        for (size_t i = 0; i < x.size(); i++) {
            max_err = max(max_err, std::abs(x[i] - x_exact[i]));
        }
        cout << "roz - dok_roz = " << scientific << setprecision(16) << max_err << "\n";
    }
}

int main() {
    cout << "zad 1" << "\n";

    Matrix A;
    Vector b;
    if (!readSystemFromFile("dane.txt", A, b)) {
        return 1;
    }

    printMatrix(A, " A");
    printVector(b, " b");

    Matrix L, U;
    if (!luDoolittle(A, L, U)) {
        cout << "macierz osobliwa numerycznie\n";
        return 1;
    }

    printMatrix(L, " L");
    printMatrix(U, " U");

    Vector y, x;
    if (!forwardSubstitution(L, b, y)) {
        cout << "nie da sie Ly = b.\n";
        return 1;
    }
    if (!backSubstitutionUpper(U, y, x)) {
        cout << "nie da sieUx = y.\n";
        return 1;
    }

    printVector(y, "roz y");
    printVector(x, "roz x");

    runScalingExperiment();

    return 0;
}