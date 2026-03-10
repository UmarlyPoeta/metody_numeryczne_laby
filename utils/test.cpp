#include "import_from_file.cpp"
#include <bits/stdc++.h>



int main() {
    std::pair<std::pair<double, double>, std::vector<std::pair<double, double>>> nums;

    nums = utils::read_typed_file("dane.txt");

    std::cout << nums.first.first << std::endl;
    
    return 0;
}