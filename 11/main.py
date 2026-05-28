import math

def f1(x):
    return -x**3 + 10*x + 5

def df1(x):
    return -3*x**2 + 10

def f2(x):
    return x**2 - 4

def df2(x):
    return 2*x


def stycznych(x0, f, df, name, *, eps=None, max_iter=None):
    print(f"Metoda stycznych: {name}")
    
    x = x0
    i = 0
    
    
    if max_iter is not None:
        for i in range(1, max_iter + 1):
            dfx = df(x)
    
            if dfx == 0:
                print("Przerwano: f'(x)=0")
                break
    
            fx = f(x)
            x -= fx / dfx
    
            print(f"It {i}: x={x}, f(x)={f(x)}")
    else:
        while eps is not None and abs(f(x)) >= eps:
    
            dfx = df(x)
            if dfx == 0:
                print("Przerwano: f'(x)=0")
                break
    
            fx = f(x)
            x -= fx / dfx
            i += 1
    
            print(f"It {i}: x={x}, f(x)={f(x)}")
    print(f"Wynik={x}, It={i}\n")
    return x


def siecznych(x0, f, name, *, eps=None, max_iter=None):
    print(f"Sieczne: {name}")
    prev = x0 - 0.1
    x = x0
    i = 0
    if max_iter is not None:
        for i in range(1, max_iter + 1):
            fx = f(x)
            fprev = f(prev)
            
            
    
            denom = fx - fprev
            if denom == 0:
                print("przez 0")
                break
    
            nx = x - fx * (x - prev) / denom
    
            prev = x
            x = nx
    
            print(f"i {i}: x={x}, f(x)={f(x)}")
    else:
        while eps is not None and abs(f(x)) >= eps:
            fx = f(x)
            fprev = f(prev)
            denom = fx - fprev
    
            if denom == 0:
                print("przez 0")
                break
            nx = x - fx * (x - prev) / denom
    
            prev = x
            x = nx
            i += 1
    
            print(f"i {i}: x={x}, f(x)={f(x)}")

    print(f"w={x}, i={i}\n")


    return x


# z czata dla ładnego debugowania
if __name__ == "__main__":
    x0 = float(input("x0 = "))
    mode = input("Tryb stopu (i=iteracje, e=epsilon) = ").strip().lower()
    if mode == "i":
        max_iter = int(input("Liczba iteracji = "))
        eps = None
    else:
        eps = float(input("eps = "))
        max_iter = None

    print("\n--- Funkcje wielomianowe ---")
    stycznych(x0, f1, df1, "-x^3 + 10x + 5 = 0", eps=eps, max_iter=max_iter)
    siecznych(x0, f1, "-x^3 + 10x + 5 = 0", eps=eps, max_iter=max_iter)

    stycznych(x0, f2, df2, "x^2 - 4 = 0", eps=eps, max_iter=max_iter)
    siecznych(x0, f2, "x^2 - 4 = 0", eps=eps, max_iter=max_iter)

