import numpy as np
import matplotlib.pyplot as plt

# =========================
# 1. Wczytaj współczynniki
# =========================
b = np.loadtxt("wspolczynniki.txt")

# =========================
# 2. Węzły (IDENTYCZNE jak w C++)
# =========================
def generate_nodes(n):
    odl = 2.0 / n
    return np.array([-1.0 + i * odl for i in range(n)])

X = generate_nodes(len(b))

# =========================
# 3. Wielomian Newtona
# =========================
def newton(x, X, b):
    result = b[0]

    for k in range(1, len(b)):
        term = b[k]
        for j in range(k):
            term *= (x - X[j])
        result += term

    return result

# =========================
# 4. Funkcja oryginalna
# =========================
def f(x):
    return 1.0 / (1 + 25 * x**2)

# =========================
# 5. Zakres wykresu
# =========================
x_vals = np.linspace(-1, 1, 500)

y_interp = np.array([newton(x, X, b) for x in x_vals])
y_true = f(x_vals)

# =========================
# 6. Rysowanie
# =========================
plt.figure(figsize=(10,6))

plt.plot(x_vals, y_interp, label="Wielomian interpolacyjny")
plt.plot(x_vals, y_true, '--', label="Funkcja oryginalna")

plt.scatter(X, f(X), color='red', zorder=5, label="Węzły")

plt.legend()
plt.grid()
plt.title("Interpolacja Newtona")
plt.savefig("interpolacja_newtona_dla_5.png")
plt.show()
