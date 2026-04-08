#include <cmath>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "../utils/import_from_file.cpp"

using Matrix = std::vector<std::vector<double>>;

namespace {

constexpr double kDefaultEps = 1e-12;
constexpr const char* kDefaultInputPath = "dane.txt";

struct QuestReport {
	int quest_id;
	std::string title;
	bool passed;
	std::string details;
};

bool approx_equal(double a, double b, double eps = 1e-9) {
	return std::abs(a - b) <= eps;
}

void print_divider() {
	std::cout << "------------------------------------------------------------\n";
}

void print_matrix(const Matrix& ab, const std::string& title) {
	std::cout << "\n" << title << "\n";
	print_divider();
	std::cout << std::fixed << std::setprecision(6);
	for (const auto& row : ab) {
		for (double value : row) {
			std::cout << std::setw(12) << value;
		}
		std::cout << "\n";
	}
}

void print_vector(const std::vector<double>& values, const std::string& title) {
	std::cout << "\n" << title << "\n";
	print_divider();
	std::cout << std::fixed << std::setprecision(8);
	for (size_t i = 0; i < values.size(); ++i) {
		std::cout << "x" << i << " = " << values[i] << "\n";
	}
}

void print_permutation(const std::vector<int>& perm, const std::string& title) {
	std::cout << "\n" << title << "\n";
	print_divider();
	for (size_t i = 0; i < perm.size(); ++i) {
		std::cout << "col[" << i << "] = " << perm[i] << "\n";
	}
}

[[maybe_unused]] bool is_rectangular(const Matrix& ab) {
	if (ab.empty()) {
		return false;
	}
	const size_t width = ab.front().size();
	if (width == 0) {
		return false;
	}
	for (const auto& row : ab) {
		if (row.size() != width) {
			return false;
		}
	}
	return true;
}

void print_course_overview() {
	std::cout << "GAUSS QUEST LAB (C++ SINGLE FILE COURSE)\n";
	print_divider();
	std::cout
		<< "This file is a training sandbox. You complete TODO blocks step by step.\n\n"
		<< "Quest list:\n"
		<< "  Quest 1: Validate augmented matrix\n"
		<< "  Quest 2: Forward elimination (no pivoting)\n"
		<< "  Quest 3: Back substitution\n"
		<< "  Quest 4: Partial pivoting by rows\n"
		<< "  Quest 5: Partial pivoting by columns (Gauss-Crout)\n"
		<< "  Quest 6: Restore solution order after column swaps\n"
		<< "  Quest 7: End-to-end sandbox run on file input\n\n"
		<< "Suggested workflow:\n"
		<< "  1) Implement one TODO function\n"
		<< "  2) Run checkpoint: ./lab check <quest_id>\n"
		<< "  3) Repeat until all pass: ./lab check all\n"
		<< "  4) Run full scenario on file: ./lab sandbox <mode> [path]\n\n"
		<< "Commands:\n"
		<< "  ./lab\n"
		<< "  ./lab check 1\n"
		<< "  ./lab check all\n"
		<< "  ./lab sandbox nopivot [dane.txt]\n"
		<< "  ./lab sandbox row [dane.txt]\n"
		<< "  ./lab sandbox col [dane.txt]\n"
		<< "  ./lab table-template\n\n"
		<< "Modes for sandbox:\n"
		<< "  nopivot - baseline Gauss (for comparison)\n"
		<< "  row     - partial pivoting by rows\n"
		<< "  col     - partial pivoting by columns (Crout style)\n\n"
		<< "Important: checkpoint code should not be changed. Only TODO zone.\n";
}

void print_table_template() {
	std::cout << "\nTask 3 comparison table template:\n\n";
	std::cout << "| Case | No pivot | Row pivot | Col pivot (Crout) | Notes |\n";
	std::cout << "|---|---|---|---|---|\n";
	std::cout << "| A1 |  |  |  |  |\n";
	std::cout << "| A2 |  |  |  |  |\n";
	std::cout << "| A3 |  |  |  |  |\n";
	std::cout << "| A4 |  |  |  |  |\n";
}

int parse_quest_id(const std::string& text) {
	std::istringstream iss(text);
	int value = -1;
	iss >> value;
	if (!iss || !iss.eof()) {
		return -1;
	}
	return value;
}

bool check_unique_permutation(const std::vector<int>& perm) {
	if (perm.empty()) {
		return false;
	}
	std::vector<int> seen(perm.size(), 0);
	for (int idx : perm) {
		if (idx < 0 || idx >= static_cast<int>(perm.size())) {
			return false;
		}
		seen[idx] += 1;
	}
	for (int count : seen) {
		if (count != 1) {
			return false;
		}
	}
	return true;
}

}  // namespace

// ============================================================================
// STUDENT TODO ZONE
// Only edit this namespace while solving quests.
// ============================================================================

namespace student {

// Quest 1
// Goal: return true for a valid augmented matrix n x (n+1), false otherwise.
// Hint: first check if matrix is rectangular, then check dimensions.
bool validate_augmented_matrix(const Matrix& ab, std::string& error) {
	// TODO(student): implement matrix shape validation.
	// Minimum checks:
	// 1) matrix is not empty
	// 2) all rows have the same number of columns
	// 3) columns == rows + 1
	error = "TODO: Quest 1 not implemented";

    if (ab.empty()) {
        return false;
    }

    int rows = ab.size();
    int columns = ab[0].size();

    for (size_t i = 0; i < ab.size(); i++) {
        if (ab[i].size() != columns) {
            return false;
        }
    }

    if (rows + 1 != columns) {
        return false;
    }

	return true;
}

// Quest 2
// Goal: perform forward elimination without pivoting.
// Hint: this method can fail if ab[k][k] is zero or close to zero.
bool forward_elimination_no_pivot(Matrix& ab, double eps, std::string& error) {
	// TODO(student): implement plain Gauss forward elimination.
	// Must transform matrix to upper-triangular form.
	// Return false with error when pivot is near zero.

    int n = ab.size();

    for (int j = 0; j < n - 1;j++) {
        double m;

        if (approx_equal(ab[j][j], 0.0, eps)) {
            return false;
        }  

        for (int i = j + 1; i < n; i++) {
            m = ab[i][j] / ab[j][j];
            for (int d = j; d <= n; d++) {
                ab[i][d] -= m * ab[j][d];
            }
        }
    }

	return true;
}

// Quest 3
// Goal: solve upper-triangular system by back substitution.
bool back_substitution(const Matrix& ab, std::vector<double>& x, double eps, std::string& error) {
	// TODO(student): implement back substitution.
	// x must have size n after successful solve.
	// Return false when diagonal element is near zero.

	size_t n = ab.size();
	x.resize(n);

	for (int i = n - 1; i >= 0; i--) {
		double sum = 0.0;

		for (int j = i + 1; j < n; j++) {
			sum += ab[i][j] * x[j];
		}

		if (std::abs(ab[i][i]) < eps) {
			return false;
		}

		x[i] = (ab[i][n] - sum) / ab[i][i];
	}

	return true;
}

// Quest 4
// Goal: forward elimination with partial pivoting by rows.
// Hint: for each k, choose row i (i>=k) maximizing abs(ab[i][k]).
bool forward_elimination_row_pivot(Matrix& ab, double eps, std::string& error) {
	// TODO(student): implement partial pivoting by rows.
	// 1) choose best pivot row in current column
	// 2) swap rows
	// 3) eliminate rows below
	int n = ab.size();
    
    for (int j = 0; j < n - 1;j++) {

		int imax = j;
		for (int i = j + 1; i < n; i++) {
			if (abs(ab[i][j]) > abs(ab[imax][j])) {
				imax = i;
			}
		}

		if (imax != j) {
			swap(ab[j], ab[imax]);
		}
        double m;

        if (approx_equal(ab[j][j], 0.0, eps)) {
            return false;
        }  

        for (int i = j + 1; i < n; i++) {
            m = ab[i][j] / ab[j][j];
            for (int d = j; d <= n; d++) {
                ab[i][d] -= m * ab[j][d];
            }
        }
    }

	return true;
}

// Quest 5
// Goal: forward elimination with partial pivoting by columns (Gauss-Crout style).
// Hint: track column permutation in col_perm.
bool forward_elimination_col_pivot(Matrix& ab,
								   std::vector<int>& col_perm,
								   double eps,
								   std::string& error) {
	// TODO(student): implement partial pivoting by columns.
	// Initialize col_perm as identity [0..n-1].
	// For each k:
	// 1) find max abs element in row k for columns j>=k
	// 2) swap columns and update col_perm
	// 3) eliminate rows below
	int n = ab.size();
    col_perm.resize(n);

	for (int i = 0; i < n; i++) {
		col_perm[i] = i;
	}

    for (int j = 0; j < n - 1;j++) {

		int jmax = j;
		for (int i = j + 1; i < n; i++) {
			if (abs(ab[j][i]) > abs(ab[j][jmax])) {
				jmax = i;
			}
		}

		if (jmax != j) {
			for (int i = 0; i < n; i++) {
				std::swap(ab[i][j], ab[i][jmax]);	
			}
			std::swap(col_perm[j], col_perm[jmax]);
		}
        double m;

        if (approx_equal(ab[j][j], 0.0, eps)) {
            return false;
        }  

        for (int i = j + 1; i < n; i++) {
            m = ab[i][j] / ab[j][j];
            for (int d = j; d <= n; d++) {
                ab[i][d] -= m * ab[j][d];
            }
        }
    }

	return true;
}

// Quest 6
// Goal: map solution from swapped-column order back to original variable order.
// Hint: if col_perm[i] == original variable index placed at i, use this mapping.
bool reorder_solution_from_col_permutation(const std::vector<double>& x_swapped,
										   const std::vector<int>& col_perm,
										   std::vector<double>& x_original,
										   std::string& error) {
	// TODO(student): implement mapping from x_swapped to x_original.
	// Validate sizes and permutation correctness.
	size_t n = x_swapped.size();
	if (n != col_perm.size()) {
		return false;
	}
	if (!check_unique_permutation(col_perm)) {
		return false;
	}

	x_original.resize(n);
	for (int i = 0; i < n; i++) {
		x_original[col_perm[i]] = x_swapped[i];
	}
	return true;
}

}  // namespace student

// ============================================================================
// CHECKPOINT ZONE
// Do not modify this section.
// ============================================================================

namespace {

QuestReport checkpoint_1_validate_matrix() {
	const int id = 1;
	const std::string title = "Validate augmented matrix";

	std::string error;

	const Matrix good = {
		{2.0, 1.0, 5.0},
		{4.0, 4.0, 6.0},
	};
	if (!student::validate_augmented_matrix(good, error)) {
		return {id, title, false, "Valid matrix rejected: " + error};
	}

	const Matrix bad_not_rect = {
		{1.0, 2.0, 3.0},
		{4.0, 5.0},
	};
	if (student::validate_augmented_matrix(bad_not_rect, error)) {
		return {id, title, false, "Invalid non-rectangular matrix accepted"};
	}

	const Matrix bad_size = {
		{1.0, 2.0, 3.0, 4.0},
		{5.0, 6.0, 7.0, 8.0},
	};
	if (student::validate_augmented_matrix(bad_size, error)) {
		return {id, title, false, "Invalid shape matrix accepted"};
	}

	return {id, title, true, "Quest 1 passed"};
}

QuestReport checkpoint_2_forward_no_pivot() {
	const int id = 2;
	const std::string title = "Forward elimination (no pivoting)";

	Matrix ab = {
		{2.0, 1.0, 5.0},
		{4.0, 4.0, 6.0},
	};

	std::string error;
	if (!student::forward_elimination_no_pivot(ab, kDefaultEps, error)) {
		return {id, title, false, "Function returned false: " + error};
	}

	if (std::abs(ab[1][0]) > 1e-9) {
		return {id, title, false, "Element below diagonal was not eliminated"};
	}

	if (std::abs(ab[0][0]) < kDefaultEps || std::abs(ab[1][1]) < kDefaultEps) {
		return {id, title, false, "Diagonal contains invalid pivot after elimination"};
	}

	return {id, title, true, "Quest 2 passed"};
}

QuestReport checkpoint_3_back_substitution() {
	const int id = 3;
	const std::string title = "Back substitution";

	const Matrix upper = {
		{2.0, 1.0, -1.0, 1.0},
		{0.0, 3.0, 2.0, 12.0},
		{0.0, 0.0, 4.0, 12.0},
	};

	std::string error;
	std::vector<double> x;
	if (!student::back_substitution(upper, x, kDefaultEps, error)) {
		return {id, title, false, "Function returned false: " + error};
	}

	if (x.size() != 3) {
		return {id, title, false, "Unexpected solution size"};
	}

	if (!approx_equal(x[0], 1.0) || !approx_equal(x[1], 2.0) || !approx_equal(x[2], 3.0)) {
		return {id, title, false, "Wrong solution values for test upper matrix"};
	}

	return {id, title, true, "Quest 3 passed"};
}

QuestReport checkpoint_4_row_pivot() {
	const int id = 4;
	const std::string title = "Partial pivoting by rows";

	Matrix ab = {
		{0.0, 2.0, 4.0},
		{5.0, 1.0, 11.0},
	};

	std::string error;
	if (!student::forward_elimination_row_pivot(ab, kDefaultEps, error)) {
		return {id, title, false, "Function returned false: " + error};
	}

	if (!approx_equal(ab[0][0], 5.0)) {
		return {id, title, false, "Row pivoting did not move the best row to top"};
	}

	if (std::abs(ab[1][0]) > 1e-9) {
		return {id, title, false, "Element below diagonal was not eliminated"};
	}

	return {id, title, true, "Quest 4 passed"};
}

QuestReport checkpoint_5_col_pivot() {
	const int id = 5;
	const std::string title = "Partial pivoting by columns (Crout)";

	Matrix ab = {
		{0.0, 9.0, 1.0, 10.0},
		{2.0, 1.0, 3.0, 9.0},
		{4.0, 2.0, 1.0, 7.0},
	};

	std::string error;
	std::vector<int> col_perm;
	if (!student::forward_elimination_col_pivot(ab, col_perm, kDefaultEps, error)) {
		return {id, title, false, "Function returned false: " + error};
	}

	if (col_perm.size() != 3 || !check_unique_permutation(col_perm)) {
		return {id, title, false, "Invalid column permutation vector"};
	}

	if (std::abs(ab[0][0]) < 1.0) {
		return {id, title, false, "Top-left pivot looks too small after column pivoting"};
	}

	if (std::abs(ab[1][0]) > 1e-9 || std::abs(ab[2][0]) > 1e-9) {
		return {id, title, false, "Elements below first pivot were not eliminated"};
	}

	return {id, title, true, "Quest 5 passed"};
}

QuestReport checkpoint_6_reorder_solution() {
	const int id = 6;
	const std::string title = "Restore original variable order";

	const std::vector<double> x_swapped = {10.0, 20.0, 30.0};
	const std::vector<int> col_perm = {2, 0, 1};

	std::string error;
	std::vector<double> x_original;
	if (!student::reorder_solution_from_col_permutation(x_swapped, col_perm, x_original, error)) {
		return {id, title, false, "Function returned false: " + error};
	}

	if (x_original.size() != 3) {
		return {id, title, false, "Unexpected output size"};
	}

	if (!approx_equal(x_original[0], 20.0) ||
		!approx_equal(x_original[1], 30.0) ||
		!approx_equal(x_original[2], 10.0)) {
		return {id, title, false, "Wrong reorder mapping"};
	}

	return {id, title, true, "Quest 6 passed"};
}

QuestReport run_checkpoint(int quest_id) {
	switch (quest_id) {
		case 1: return checkpoint_1_validate_matrix();
		case 2: return checkpoint_2_forward_no_pivot();
		case 3: return checkpoint_3_back_substitution();
		case 4: return checkpoint_4_row_pivot();
		case 5: return checkpoint_5_col_pivot();
		case 6: return checkpoint_6_reorder_solution();
		default:
			return {quest_id, "Unknown", false, "Unknown quest id"};
	}
}

std::vector<QuestReport> run_all_checkpoints() {
	std::vector<QuestReport> reports;
	for (int quest_id = 1; quest_id <= 6; ++quest_id) {
		reports.push_back(run_checkpoint(quest_id));
	}
	return reports;
}

void print_report(const QuestReport& report) {
	std::cout << "[Quest " << report.quest_id << "] " << report.title << " -> "
			  << (report.passed ? "PASS" : "FAIL") << "\n";
	std::cout << "  " << report.details << "\n";
}

int handle_check_command(int argc, char** argv) {
	if (argc < 3) {
		std::cout << "Usage: ./lab check <quest_id|all>\n";
		return 1;
	}

	const std::string arg = argv[2];
	if (arg == "all") {
		const auto reports = run_all_checkpoints();
		bool all_passed = true;
		for (const auto& report : reports) {
			print_report(report);
			all_passed = all_passed && report.passed;
		}
		return all_passed ? 0 : 2;
	}

	const int quest_id = parse_quest_id(arg);
	if (quest_id < 1 || quest_id > 6) {
		std::cout << "Quest id must be in range 1..6\n";
		return 1;
	}

	const auto report = run_checkpoint(quest_id);
	print_report(report);
	return report.passed ? 0 : 2;
}

int handle_sandbox_command(int argc, char** argv) {
	if (argc < 3) {
		std::cout << "Usage: ./lab sandbox <nopivot|row|col> [path]\n";
		return 1;
	}

	const std::string mode = argv[2];
	const std::string path = (argc >= 4) ? argv[3] : kDefaultInputPath;

	Matrix ab;
	try {
		ab = utils::read_matrix(path);
	} catch (const std::exception& ex) {
		std::cout << "Input error: " << ex.what() << "\n";
		return 1;
	}

	std::string error;
	if (!student::validate_augmented_matrix(ab, error)) {
		std::cout << "Validation failed: " << error << "\n";
		std::cout << "Hint: implement Quest 1 first.\n";
		return 2;
	}

	print_matrix(ab, "Extended matrix before calculations");

	std::vector<int> col_perm;
	bool ok = false;
	if (mode == "nopivot") {
		ok = student::forward_elimination_no_pivot(ab, kDefaultEps, error);
	} else if (mode == "row") {
		ok = student::forward_elimination_row_pivot(ab, kDefaultEps, error);
	} else if (mode == "col") {
		ok = student::forward_elimination_col_pivot(ab, col_perm, kDefaultEps, error);
	} else {
		std::cout << "Unknown mode. Use: nopivot, row, col\n";
		return 1;
	}

	if (!ok) {
		std::cout << "Forward stage failed: " << error << "\n";
		return 2;
	}

	print_matrix(ab, "Extended matrix after forward elimination");

	if (mode == "col") {
		print_permutation(col_perm, "Column permutation vector");
	}

	std::vector<double> x;
	if (!student::back_substitution(ab, x, kDefaultEps, error)) {
		std::cout << "Back substitution failed: " << error << "\n";
		std::cout << "Hint: implement Quest 3.\n";
		return 2;
	}

	if (mode == "col") {
		std::vector<double> x_original;
		if (!student::reorder_solution_from_col_permutation(x, col_perm, x_original, error)) {
			std::cout << "Reorder failed: " << error << "\n";
			std::cout << "Hint: implement Quest 6.\n";
			return 2;
		}
		print_vector(x_original, "Solution in original variable order");
	} else {
		print_vector(x, "Solution vector");
	}

	return 0;
}

}  // namespace

int main(int argc, char** argv) {
	if (argc == 1) {
		print_course_overview();
		return 0;
	}

	const std::string command = argv[1];

	if (command == "check") {
		return handle_check_command(argc, argv);
	}

	if (command == "sandbox") {
		return handle_sandbox_command(argc, argv);
	}

	if (command == "table-template") {
		print_table_template();
		return 0;
	}

	std::cout << "Unknown command: " << command << "\n\n";
	print_course_overview();
	return 1;
}
