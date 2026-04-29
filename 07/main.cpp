#include <iostream>
#include <vector>
#include <cmath>
#include <iostream>
#include <cmath>
#include <iomanip>
#include <string>


using namespace std;

using namespace std;

using Func = double(*)(double);



double sinf(double x) {
    return sin(x);
}

double f_sin(double x) {
    return sin(x);
}

double f_poly(double x) {
    return x * x + 2 * x + 5;
}

double f_exp(double x) {
    return exp(x);
}

double rect(Func f, double a, double b, int n) {
    double h = (b - a) / n;
    double sum = 0.0;
    for (int i = 0; i < n; ++i) {
        double x = a + i * h;
        sum += f(x + 0.5 * h);
    }
    return sum * h;
}

double trap(Func f, double a, double b, int n) {
    double h = (b - a) / n;
    double sum = 0.5 * (f(a) + f(b));
    for (int i = 1; i < n; ++i) {
        sum += f(a + i * h);
    }
    return sum * h;
}

double simpson(Func f, double a, double b, int n) {
    double h = (b - a) / n;
    double sum = f(a) + f(b);
    for (int i = 1; i < n; ++i) {
        double x = a + i * h;
        sum += (i % 2 == 0 ? 2.0 : 4.0) * f(x);
    }
    return sum * h / 3.0;
}

void run_case(const string& label, Func f, double a, double b, int n) {
    double r = rect(f, a, b, n);
    double t = trap(f, a, b, n);
    double s = simpson(f, a, b, n);
    cout << "f=" << label << " a=" << a << " b=" << b << " n=" << n
         << " r=" << r << " t=" << t << " s=" << s << "\n";
}

int main() {
    cout.setf(ios::fixed);
    cout << setprecision(6);

    int n = 4;
    run_case("sin(x)", f_sin, 0.5, 2.5, n);
    run_case("x^2+2x+5", f_poly, 0.5, 5.0, n);
    run_case("exp(x)", f_exp, 0.5, 5.0, n);

    return 0;
}