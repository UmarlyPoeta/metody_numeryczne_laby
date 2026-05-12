def euler(x_docel, y, a, b, N, f):
    h = (b - a) / N

    print("euler\n")

    x = a
    for _ in range(N):
        y = y + h * f(x, y)
        x = x + h
        print(f"x = {x}, y = {y}")
    print(f"y({x_docel}) = {y}\n")


def rung_kutta_rk2(x_docel, y, a, b, N, f):
    k1 = 0
    k2 = 0
    h = (b - a) / N

    x = a
    print("rung_kutta_rk2\n")
    for _ in range(N):
        k1 = f(x, y)
        k2 = f(x + h, y + h * k1)
        y = y + h * (k1 + k2) / 2
        x = x + h
        print(f"x = {x}, y = {y}")
    print(f"y({x_docel}) = {y}\n")


def rung_kutta_rk4(x_docel, y, a, b, N, f):
    k1 = 0
    k2 = 0
    k3 = 0
    k4 = 0
    h = (b - a) / N

    x = a
    print("rung_kutta_rk4\n")
    for _ in range(N):
        k1 = f(x, y)
        k2 = f(x + h / 2, y + h * k1 / 2)
        k3 = f(x + h / 2, y + h * k2 / 2)
        k4 = f(x + h, y + h * k3)
        y = y + h * (k1 + 2 * k2 + 2 * k3 + k4) / 6
        x = x + h
        print(f"x = {x}, y = {y}")
    print(f"y({x_docel}) = {y}\n")


def main():
    func1 = lambda x, y: 2 * y * (x + 1)
    y_00 = 1.0

    func2 = lambda x, y: x + y
    y_01 = 0.1

    N = 10
    x = 1

    euler(x, y_00, 0, 1, N, func1)
    rung_kutta_rk2(x, y_00, 0, 1, N, func1)
    rung_kutta_rk4(x, y_00, 0, 1, N, func1)

    euler(x, y_01, 0, 1, N, func2)
    rung_kutta_rk2(x, y_01, 0, 1, N, func2)
    rung_kutta_rk4(x, y_01, 0, 1, N, func2)


if __name__ == "__main__":
    main()
