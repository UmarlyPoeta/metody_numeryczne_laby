import math



# dla wygody poproszony czat o sprowadzenie tabelki do dicta
GL_NODES_WEIGHTS = {
    2: (
        [-1 / math.sqrt(3), 1 / math.sqrt(3)],
        [1.0, 1.0],
    ),
    3: (
        [-math.sqrt(3 / 5), 0.0, math.sqrt(3 / 5)],
        [5 / 9, 8 / 9, 5 / 9],
    ),
    4: (
        [
            -math.sqrt((3 + 2 * math.sqrt(6 / 5)) / 7),
            -math.sqrt((3 - 2 * math.sqrt(6 / 5)) / 7),
            math.sqrt((3 - 2 * math.sqrt(6 / 5)) / 7),
            math.sqrt((3 + 2 * math.sqrt(6 / 5)) / 7),
        ],
        [
            (18 - math.sqrt(30)) / 36,
            (18 + math.sqrt(30)) / 36,
            (18 + math.sqrt(30)) / 36,
            (18 - math.sqrt(30)) / 36,
        ],
    ),
}


def gauss_legendre(f, a, b, n):
    nodes, weights = GL_NODES_WEIGHTS[n]
    mid = 0.5 * (a + b)
    half = 0.5 * (b - a)
    s = 0.0
    for x, w in zip(nodes, weights):
        t = mid + half * x
        s += w * f(t)
    return half * s


def rect_left(f, a, b, n):
    h = (b - a) / n
    return sum(f(a + i * h) for i in range(n)) * h


def trapezoid(f, a, b, n):
    h = (b - a) / n
    s = 0.5 * (f(a) + f(b))
    for i in range(1, n):
        s += f(a + i * h)
    return s * h


def simpson(f, a, b, n):
    if n % 2 == 1:
        n += 1
    h = (b - a) / n
    s = f(a) + f(b)
    for i in range(1, n):
        coef = 4 if i % 2 == 1 else 2
        s += coef * f(a + i * h)
    return s * h / 3


def main():
    n_comp = 20

    tasks1 = [
                                        # dokladne wyniki calkowania z czata
        ("sin(x)", math.sin, 0.5, 2.5, lambda: -math.cos(2.5) + math.cos(0.5)),
        (
            "x^2+2x+5",
            lambda x: x * x + 2 * x + 5,
            0.5,
            5.0,
            lambda: (5**3 - 0.5**3) / 3 + (5**2 - 0.5**2) + 5 * (5 - 0.5),
        ),
        ("exp(x)", math.exp, 0.5, 5.0, lambda: math.exp(5.0) - math.exp(0.5)),
    ]

    print("zad 1")
    for name, f, a, b, exact_fn in tasks1:
        exact = exact_fn()
        gl2 = gauss_legendre(f, a, b, 2)
        gl3 = gauss_legendre(f, a, b, 3)
        gl4 = gauss_legendre(f, a, b, 4)
        r = rect_left(f, a, b, n_comp)
        t = trapezoid(f, a, b, n_comp)
        s = simpson(f, a, b, n_comp)
        print(f"{name} [{a}, {b}]")
        print(f"  GL2: {gl2:.6f}  GL3: {gl3:.6f}  GL4: {gl4:.6f}")
        print(f"  pr: {r:.6f}  tr: {t:.6f}  pa: {s:.6f}")
        print(f"  dok: {exact:.6f}\n")

    tasks2 = [
        ("I1", lambda x: 3 * x * x + 2 * x + 1, -1.0, 1.0),
        ("I2", lambda x: x**3 - 4 * x + 2, -1.0, 1.0),
        ("I3", lambda x: x**4 + x**2 + 1, -1.0, 1.0),
        ("I4", lambda x: x**5 - 2 * x**3 + x, -1.0, 1.0),
        ("I5", lambda x: x**6 + x**2, -1.0, 1.0),
        ("I6", lambda x: x**4 - 3 * x**2 + 2, 0.0, 2.0),
        ("I7", math.exp, 0.0, 1.0),
        ("I8", math.sin, 0.0, math.pi),
    ]

    print("zad 2")
    for name, f, a, b in tasks2:
        gl2 = gauss_legendre(f, a, b, 2)
        gl3 = gauss_legendre(f, a, b, 3)
        gl4 = gauss_legendre(f, a, b, 4)
        print(f"{name}: GL2={gl2:.6f} GL3={gl3:.6f} GL4={gl4:.6f}")



    # 1 dokladne dla <= 3
    # 2 dokladnie dla <= 5
    # 3 dokładnie dla <= 7
    # 4 exp i sin nie sa wielomianami, a metoda daje lepsza dokl. dla stopnia 2n - 1
    # 5 blad maleje ze wzrostem liczby wezlow

if __name__ == "__main__":
    main()