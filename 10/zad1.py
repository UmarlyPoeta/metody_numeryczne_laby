import math
import sys
from typing import List, Tuple


def safe_eval(expr: str, x_value: float) -> float:
    allowed = {
        name: getattr(math, name) for name in dir(math) if not name.startswith("_")
    }
    allowed["x"] = x_value
    return float(eval(expr, {"__builtins__": {}}, allowed))


def parse_input(path: str) -> Tuple[int, int, List[float], List[float], List[float]]:
    with open(path, "r", encoding="utf-8") as f:
        lines = [
            line.strip()
            for line in f
            if line.strip() and not line.strip().startswith("#")
        ]

    if len(lines) < 5:
        raise ValueError("Plik wejściowy musi mieć co najmniej 5 niepustych linii.")

    degree = int(lines[0].split()[0])
    n = int(lines[1].split()[0])

    x_vals = list(map(float, lines[2].split()))
    y_vals = list(map(float, lines[3].split()))

    if len(x_vals) != n or len(y_vals) != n:
        raise ValueError("Liczba węzłów lub wartości funkcji nie zgadza się z N.")

    weight_line = lines[4]

    if weight_line.lower().startswith(("expr:", "w=", "weight_expr:")):
        if ":" in weight_line:
            expr = weight_line.split(":", 1)[1].strip()
        else:
            expr = weight_line.split("=", 1)[1].strip()
        weights = [safe_eval(expr, x) for x in x_vals]
    else:
        weights = list(map(float, weight_line.split()))
        if len(weights) != n:
            raise ValueError("Liczba wag nie zgadza się z N.")

    return degree, n, x_vals, y_vals, weights


def build_system(
    degree: int,
    x_vals: List[float],
    y_vals: List[float],
    weights: List[float],
) -> Tuple[List[List[float]], List[float]]:
    size = degree + 1
    g = [[0.0 for _ in range(size)] for _ in range(size)]
    F = [0.0 for _ in range(size)]

    for x, y, w in zip(x_vals, y_vals, weights):
        powers = [1.0]
        for _ in range(1, 2 * degree + 1):
            powers.append(powers[-1] * x)

        for k in range(size):
            F[k] += w * y * powers[k]
            for j in range(size):
                g[k][j] += w * powers[k + j]

    return g, F


def solve_gauss(A: List[List[float]], b: List[float]) -> List[float]:
    n = len(b)
    M = [row[:] for row in A]
    v = b[:]

    for k in range(n):
        pivot = max(range(k, n), key=lambda i: abs(M[i][k]))
        if abs(M[pivot][k]) < 1e-12:
            raise ValueError("Układ osobliwy lub źle uwarunkowany.")
        if pivot != k:
            M[k], M[pivot] = M[pivot], M[k]
            v[k], v[pivot] = v[pivot], v[k]

        for i in range(k + 1, n):
            factor = M[i][k] / M[k][k]
            if factor == 0.0:
                continue
            for j in range(k, n):
                M[i][j] -= factor * M[k][j]
            v[i] -= factor * v[k]

    x = [0.0 for _ in range(n)]
    for i in range(n - 1, -1, -1):
        s = v[i]
        for j in range(i + 1, n):
            s -= M[i][j] * x[j]
        x[i] = s / M[i][i]
    return x


def evaluate_polynomial(coeffs: List[float], x: float) -> float:
    result = 0.0
    power = 1.0
    for a in coeffs:
        result += a * power
        power *= x
    return result


def approximation_error(
    x_vals: List[float],
    y_vals: List[float],
    weights: List[float],
    coeffs: List[float],
) -> float:
    error = 0.0
    for x, y, w in zip(x_vals, y_vals, weights):
        y_hat = evaluate_polynomial(coeffs, x)
        error += w * (y - y_hat) ** 2
    return error


def main() -> None:
    degree, n, x_vals, y_vals, weights = parse_input("dane.txt")

    if n < degree + 1:
        raise ValueError("liczba wezlow >= stopien + 1")

    A, b = build_system(degree, x_vals, y_vals, weights)
    coeffs = solve_gauss(A, b)

    print(f"Liczba węzłów: {n}")
    print("wsp wiel aproks:")
    for i, a in enumerate(coeffs):
        print(f"a{i} = {a}")

    print("\nwezly, argumenty, i aproksymacja:")
    print("x\t y\t F(x)")
    for x, y in zip(x_vals, y_vals):
        y_hat = evaluate_polynomial(coeffs, x)
        print(f"{x}\t {y}\t {y_hat}")

    error = approximation_error(x_vals, y_vals, weights, coeffs)
    print(f"\nE = {error}")


if __name__ == "__main__":
    main()
