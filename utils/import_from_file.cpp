#ifndef IMPORT_FROM_FILE_HPP
#define IMPORT_FROM_FILE_HPP

#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <stdexcept>
#include <iostream>

namespace utils {

    /**
     * Wczytuje cały plik tekstowy do stringa.
     */
    inline std::string read_file_to_string(const std::string& filepath) {
        std::ifstream file(filepath);
        if (!file.is_open()) {
            throw std::runtime_error("Nie można otworzyć pliku: " + filepath);
        }
        std::stringstream ss;
        ss << file.rdbuf();
        return ss.str();
    }

    inline std::pair<std::pair<int,int>, std::vector<std::pair<double, double>>> read_typed_file(const std::string& filepath) {
        std::ifstream file(filepath);
        if (!file.is_open()) {
            throw std::runtime_error("Nie można otworzyć pliku: " + filepath);
        }
        int n, m;
        if (!(file >> n >> m)) {
            throw std::runtime_error("Nie można wczytać nagłówka (n, m) z pliku: " + filepath);
        }
        std::vector<std::pair<double, double>> data;
        double x, y;
        while (file >> x >> y) {
            data.emplace_back(x, y);
        }
        if (static_cast<int>(data.size()) != n) {
            std::cerr << "Ostrzeżenie: oczekiwano " << n << " wierszy danych, wczytano " << data.size() << "\n";
        }
        return {{n, m}, data};
    }

    /**
     * Wczytuje plik tekstowy linia po linii do wektora stringów.
     */
    inline std::vector<std::string> read_lines(const std::string& filepath) {
        std::ifstream file(filepath);
        if (!file.is_open()) {
            throw std::runtime_error("Nie można otworzyć pliku: " + filepath);
        }
        std::vector<std::string> lines;
        std::string line;
        while (std::getline(file, line)) {
            lines.push_back(line);
        }
        return lines;
    }

    /**
     * Wczytuje wektor wartości double z pliku (jedna wartość na linię).
     */
    inline std::vector<double> read_doubles(const std::string& filepath) {
        std::ifstream file(filepath);
        if (!file.is_open()) {
            throw std::runtime_error("Nie można otworzyć pliku: " + filepath);
        }
        std::vector<double> values;
        double val;
        while (file >> val) {
            values.push_back(val);
        }
        return values;
    }

    /**
     * Wczytuje wektor wartości int z pliku (jedna wartość na linię).
     */
    inline std::vector<int> read_ints(const std::string& filepath) {
        std::ifstream file(filepath);
        if (!file.is_open()) {
            throw std::runtime_error("Nie można otworzyć pliku: " + filepath);
        }
        std::vector<int> values;
        int val;
        while (file >> val) {
            values.push_back(val);
        }
        return values;
    }

    /**
     * Wczytuje macierz double z pliku tekstowego.
     * Każda linia to jeden wiersz, wartości oddzielone separatorem (domyślnie spacja/tab).
     */
    inline std::vector<std::vector<double>> read_matrix(const std::string& filepath, char delimiter = ' ') {
        std::ifstream file(filepath);
        if (!file.is_open()) {
            throw std::runtime_error("Nie można otworzyć pliku: " + filepath);
        }
        std::vector<std::vector<double>> matrix;
        std::string line;
        while (std::getline(file, line)) {
            if (line.empty()) continue;
            std::vector<double> row;
            std::stringstream ss(line);
            std::string token;
            while (std::getline(ss, token, delimiter)) {
                if (!token.empty()) {
                    try {
                        row.push_back(std::stod(token));
                    } catch (const std::exception&) {
                        // pomiń niepoprawne wartości
                    }
                }
            }
            if (!row.empty()) {
                matrix.push_back(row);
            }
        }
        return matrix;
    }


    /**
     * Wczytuje pary (x, y) z pliku — dwie kolumny double.
     */
    inline std::vector<std::pair<double, double>> read_xy_pairs(const std::string& filepath) {
        std::ifstream file(filepath);
        if (!file.is_open()) {
            throw std::runtime_error("Nie można otworzyć pliku: " + filepath);
        }
        std::vector<std::pair<double, double>> pairs;
        double x, y;
        while (file >> x >> y) {
            pairs.emplace_back(x, y);
        }
        return pairs;
    }

    /**
     * Wypisuje macierz na standardowe wyjście (do debugowania).
     */
    inline void print_matrix(const std::vector<std::vector<double>>& matrix) {
        for (const auto& row : matrix) {
            for (size_t i = 0; i < row.size(); ++i) {
                if (i > 0) std::cout << "\t";
                std::cout << row[i];
            }
            std::cout << "\n";
        }
    }

    /**
     * Zapisuje wektor double do pliku (jedna wartość na linię).
     */
    inline void write_doubles(const std::string& filepath, const std::vector<double>& values) {
        std::ofstream file(filepath);
        if (!file.is_open()) {
            throw std::runtime_error("Nie można otworzyć pliku do zapisu: " + filepath);
        }
        for (const auto& val : values) {
            file << val << "\n";
        }
    }

    /**
     * Zapisuje macierz double do pliku.
     */
    inline void write_matrix(const std::string& filepath, const std::vector<std::vector<double>>& matrix, char delimiter = ' ') {
        std::ofstream file(filepath);
        if (!file.is_open()) {
            throw std::runtime_error("Nie można otworzyć pliku do zapisu: " + filepath);
        }
        for (const auto& row : matrix) {
            for (size_t i = 0; i < row.size(); ++i) {
                if (i > 0) file << delimiter;
                file << row[i];
            }
            file << "\n";
        }
    }

} // namespace utils

#endif // IMPORT_FROM_FILE_HPP