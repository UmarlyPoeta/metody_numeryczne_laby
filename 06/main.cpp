#include <iostream>
#include <vector>
#include <cmath>
#include <iomanip>
#include <algorithm>
#include <string>
#include <utility>
#include <fstream>
#include "../utils/import_from_file.cpp"

using namespace std;

typedef vector<vector<double>> Matrix;
typedef vector<double> Vector;
typedef vector<int> IndexVector;

#include <chrono>

template<typename Func>
double measureTime(Func f) {
    auto start = std::chrono::high_resolution_clock::now();
    f();
    auto end = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double> diff = end - start;
    return diff.count(); // sekundy
}


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


// blad bezwzgledny
Vector error(const Vector& x1, const Vector& x2) {
    int n = x1.size();
    Vector err(n);

    for (int i = 0; i < n; i++) {
        err[i] = std::abs(x1[i] - x2[i]);
    }
    return err;
}


// Jacobian 
void splitMatrix(const Matrix& A, Matrix& L, Matrix& D, Matrix& U) {
    int n = A.size();
    L.assign(n, Vector(n, 0.0));
    D.assign(n, Vector(n, 0.0));
    U.assign(n, Vector(n, 0.0));

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (i > j) L[i][j] = A[i][j];
            else if (i < j) U[i][j] = A[i][j];
            else D[i][j] = A[i][j];
        }
    }
}

bool isWeaklyDominant(const Matrix& A) {
    int n = A.size();

    for (int i = 0; i < n; i++) {
        double sum = 0.0;
        for (int j = 0; j < n; j++) {
            if (j != i) sum += std::abs(A[i][j]);
        }

        if (std::abs(A[i][i]) < sum) return false;
    }
    return true;
}

Matrix inverseDiagonal(const Matrix& D) {
    int n = D.size();
    Matrix Dinv(n, Vector(n, 0.0));

    for (int i = 0; i < n; i++) {
        Dinv[i][i] = 1.0 / D[i][i];
    }
    return Dinv;
}

Vector jacobi(const Matrix& L, const Matrix& U, const Matrix& Dinv,
                        const Vector& b, double eps, int maxIter, int& performedIter) {
    
    int n = b.size();
    Vector x(n, 0.0);
    Vector x_new(n, 0.0);

    performedIter = 0;

    for (int it = 0; it < maxIter; it++) {
        bool stop = true;

        for (int i = 0; i < n; i++) {
            double sum = 0.0;

            // (L + U) * x
            for (int j = 0; j < n; j++) {
                sum += (L[i][j] + U[i][j]) * x[j];
            }

            // x_new = -D^{-1} * (L+U)x + D^{-1}b
            x_new[i] = -Dinv[i][i] * sum + Dinv[i][i] * b[i];

            if (std::abs(x_new[i] - x[i]) >= eps) {
                stop = false;
            }
        }

        x = x_new;
        performedIter++;

        if (stop) break;
    }

    return x;
}


/**/

// Gauss


Vector gauss(const Matrix& A, const Vector& b) {
    int n = A.size();
    const double EPS = 1e-12;

    if (n == 0 || b.size() != static_cast<size_t>(n)) {
        std::cout << "Niepoprawny uklad: niezgodny rozmiar A i b\n";
        return {};
    }

    for (int i = 0; i < n; i++) {
        if (A[i].size() != static_cast<size_t>(n)) {
            std::cout << "Niepoprawny uklad: macierz A nie jest kwadratowa\n";
            return {};
        }
    }

    Matrix X(n, Vector(n + 1, 0.0));
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            X[i][j] = A[i][j];
        }
        X[i][n] = b[i];
    }


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
            return {};
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
            return {};
        }

        x[i] = (X[i][n] - sum) / X[i][i];
    }



    return x;
}

/**/

Matrix generateDiagonallyDominantMatrix(int n) { // kod z czata do generowania macierzy
    Matrix A(n, Vector(n, 0.0));

    for (int i = 0; i < n; i++) {
        double rowSum = 0.0;

        for (int j = 0; j < n; j++) {
            if (i != j) {
                A[i][j] = (rand() % 10 + 1); // 1..10
                rowSum += std::abs(A[i][j]);
            }
        }

        A[i][i] = rowSum + (rand() % 10 + 1);
    }

    return A;
}

Vector generateVector(int n) {
    Vector b(n);
    for (int i = 0; i < n; i++) {
        b[i] = rand() % 100;
    }
    return b;
}

Vector matVec(const Matrix& A, const Vector& x) {
    int n = A.size();
    Vector result(n, 0.0);

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            result[i] += A[i][j] * x[j];
        }
    }

    return result;
}

double norm(const Vector& v) {
    double sum = 0.0;
    for (double val : v) {
        sum += val * val;
    }
    return std::sqrt(sum);
}

double residual(const Matrix& A, const Vector& x, const Vector& b) {
    Vector Ax = matVec(A, x);

    Vector diff(A.size());
    for (int i = 0; i < A.size(); i++) {
        diff[i] = Ax[i] - b[i];
    }

    return norm(diff);
}

void saveResultsToCSV(const string& filename,
                      double eps,
                      int iter,
                      double time_jacobi,
                      double residual_val,
                      double time_gauss,
                      bool append = true) 
{
    ofstream file;

    if (append)
        file.open(filename, ios::app);
    else
        file.open(filename);

    if (!file.is_open()) {
        cerr << "Nie mozna otworzyc pliku!\n";
        return;
    }

    // nagłówek tylko raz
    if (!append) {
        file << "eps,iteracje,time_jacobi,residual,time_gauss\n";
    }

    file << eps << ","
         << iter << ","
         << time_jacobi << ","
         << residual_val << ","
         << time_gauss << "\n";

    file.close();
}

void runExperiment(const Matrix& A, const Vector& b) {
    vector<double> eps_values = {0.001, 0.000001};
    int maxIter = 10000;

    string filename = "wyniki.csv";

    // nadpisz plik i dodaj nagłówek
    saveResultsToCSV(filename, 0, 0, 0, 0, 0, false);


    Matrix L;
    Matrix U;
    Matrix D;

    splitMatrix(A, L, D, U);

    for (double eps : eps_values) {

        int iter = 0;
        Vector x_j;

        double time_jacobi = measureTime([&]() {
            x_j = jacobi(L, U, inverseDiagonal(D), b, eps, maxIter, iter);
        });

        double res = residual(A, x_j, b);

        Vector x_g;
        double time_gauss = measureTime([&]() {
            x_g = gauss(A, b);
        });

        // zapis do pliku
        saveResultsToCSV(filename, eps, iter, time_jacobi, res, time_gauss, true);
    }

    cout << "Zapisano wyniki do pliku: " << filename << endl;
}

int main() {
    int ilosc_iteracji;

    Matrix A;
    Vector B;
    readSystemFromFile("dane.txt", A, B);


    printMatrix(A, "mac A");



    cout << " ----- JACOB -----\n\n";

    if (isWeaklyDominant(A)) {
        cout << "slabo dominujaca\n";
    }

    cout << "ilosc iteracji: ";
    cin >> ilosc_iteracji; 


    Matrix L, U, D;

    splitMatrix(A, L, D, U);

    Matrix LU = L;

    for (int i = 0; i < L.size(); i++) {
        for (int j = 0; j < L[0].size(); j++) {
            LU[i][j] += U[i][j];
        }
    }

    printMatrix(LU, "mac L + U");

    Matrix D_od = inverseDiagonal(D);

    printMatrix(D_od, "mac D^-1");

    int iter = 0;

    Vector X = jacobi(L, U, D_od, B, 1, ilosc_iteracji, iter);


    string r = "Rozw po " + to_string(ilosc_iteracji);

    printVector(X, r);


    cout << " ----- GAUSS -----\n\n";

    Vector X_g = gauss(A, B);

    printVector(X_g, "Rozw gauss");

    cout << "-- blad bezwzgledny -- " << endl;

    printVector(error(X, X_g), "blad bezwzgledny");


    //zad2

    cout << "----- ZAD 2 ------\n\n";


    int maxIter = 10000;

    Matrix l, u, d;

    splitMatrix(A, l, d, u);

    for (double eps : {0.001, 0.000001}) {
        cout << "\n==== EPS = " << eps << " ====\n";

        int iteracje = 0;
        Vector X = jacobi(l, u, inverseDiagonal(d), B, eps, maxIter, iteracje);

        cout << "Liczba iteracji: " << iteracje << "\n";
        printVector(X, "Rozwiazanie");

        Vector X_g = gauss(A, B);

        cout << "Blad bezwzgledny:\n";
        printVector(error(X, X_g), "blad");
    }

    /*
    cout << "------- ZAD 3 -------\n\n";

    int n = 1000;

    Matrix A_gen = generateDiagonallyDominantMatrix(n);
    Vector b_gen = generateVector(n);

    runExperiment(A_gen, b_gen);
    */
    return 0;
}