def copy_matrix(a):
	return [row[:] for row in a]


def back_substitution(a, eps=1e-15):
	n = len(a)
	x = [0.0] * n
	for i in range(n - 1, -1, -1):
		if abs(a[i][i]) < eps:
			return None
		s = 0.0
		for j in range(i + 1, n):
			s += a[i][j] * x[j]
		x[i] = (a[i][n] - s) / a[i][i]
	return x


def solve_no_pivot(ab, eps=1e-15):
	a = copy_matrix(ab)
	n = len(a)
	for k in range(n - 1):
		if abs(a[k][k]) < eps:
			return None
		for i in range(k + 1, n):
			factor = a[i][k] / a[k][k]
			for j in range(k, n + 1):
				a[i][j] -= factor * a[k][j]
	return back_substitution(a, eps)


def solve_partial(ab, eps=1e-15):
	a = copy_matrix(ab)
	n = len(a)
	for k in range(n - 1):
		max_row = k
		for i in range(k + 1, n):
			if abs(a[i][k]) > abs(a[max_row][k]):
				max_row = i
		if abs(a[max_row][k]) < eps:
			return None
		if max_row != k:
			a[k], a[max_row] = a[max_row], a[k]
		for i in range(k + 1, n):
			factor = a[i][k] / a[k][k]
			for j in range(k, n + 1):
				a[i][j] -= factor * a[k][j]
	return back_substitution(a, eps)


def solve_crout(ab, eps=1e-15):
	a = copy_matrix(ab)
	n = len(a)
	order = list(range(n))
	for k in range(n - 1):
		max_col = k
		for j in range(k + 1, n):
			if abs(a[k][j]) > abs(a[k][max_col]):
				max_col = j
		if abs(a[k][max_col]) < eps:
			return None
		if max_col != k:
			for i in range(n):
				a[i][k], a[i][max_col] = a[i][max_col], a[i][k]
			order[k], order[max_col] = order[max_col], order[k]
		for i in range(k + 1, n):
			factor = a[i][k] / a[k][k]
			for j in range(k, n + 1):
				a[i][j] -= factor * a[k][j]
	temp = back_substitution(a, eps)
	if temp is None:
		return None
	x = [0.0] * n
	for i, val in enumerate(temp):
		x[order[i]] = val
	return x


def format_row(cells, widths):
	return "| " + " | ".join(str(cell).ljust(width) for cell, width in zip(cells, widths)) + " |"


def print_table(headers, rows):
	widths = [len(h) for h in headers]
	for row in rows:
		for i, cell in enumerate(row):
			widths[i] = max(widths[i], len(str(cell)))
	print(format_row(headers, widths))
	print("| " + " | ".join("-" * width for width in widths) + " |")
	for row in rows:
		print(format_row(row, widths))


def vec_text(x):
	if x is None:
		return "blad"
	return "[" + ", ".join(f"{v:.6f}" for v in x) + "]"


def build_cases():
	return [
		("zad1", [[1e-20, 1.0, 1.0], [1.0, 1.0, 2.0]]),
		("zad2", [[1.0, 1e8, 1e8, 2e8 + 1.0], [1.0, 1e8 + 1.0, 1e8, 2e8 + 2.0], [1.0, 1e8, 1e8 + 1.0, 2e8 + 2.0]]),
		("zad3", [[1.0, 1.0, 1.0, 3.0], [1.0, 1.0000001, 1.0, 3.0000001], [1.0, 1.0, 1.0000001, 3.0000001]]),
		("zad4", [[1.0, 1e10, 1.0, 1e10 + 2.0], [1.0, 1e10 + 1.0, 1.0, 1e10 + 3.0], [1.0, 1e10, 2.0, 1e10 + 3.0]]),
		("zad5", [[1.0, 1.0, 1.0, 1.0, 4.0], [1.0, 1.0001, 1.00020001, 1.000300030001, 4.000600040001], [1.0, 1.0002, 1.00040004, 1.000600120008, 4.001200160008], [1.0, 1.0003, 1.00060009, 1.000900270027, 4.001800360027]]),
		("zad6", [[1.0, 1.0, 2.0], [1.0, 1.00000000000001, 2.00000000000001]]),
	]


def main():
	headers = ["Przypadek", "Bez pivot (x)", "Partial (x)", "Crout (x)"]
	rows = []
	for name, ab in build_cases():
		r0 = vec_text(solve_no_pivot(ab))
		r1 = vec_text(solve_partial(ab))
		r2 = vec_text(solve_crout(ab))
		rows.append([name, r0, r1, r2])
	print_table(headers, rows)


if __name__ == "__main__":
	main()
