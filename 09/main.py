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
        x = x + h
        k1 = f(x, y)
        k2 = f(x + h, y + h * k1)
        y = y + h * k2
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
        x = x + h
        k1 = f(x, y)
        k2 = f(x + h / 2, y + h * k1 / 2)
        k3 = f(x + h / 2, y + h * k2 / 2)
        k4 = f(x + h, y + h * k3)
        y = y + h * (k1 + 2 * k2 + 2 * k3 + k4) / 6
        print(f"x = {x}, y = {y}")
    print(f"y({x_docel}) = {y}\n")


def main():
    func = lambda x, y: pow(x, 2) + y

    a = float(input("a = "))
    b = float(input("b = "))
    N = int(input("N = "))
    y_0 = float(input("y_0 = "))
    x_docel = float(input("x_docel = "))

    euler(x_docel, y_0, a, b, N, func)
    rung_kutta_rk2(x_docel, y_0, a, b, N, func)
    rung_kutta_rk4(x_docel, y_0, a, b, N, func)


if __name__ == "__main__":
    main()
