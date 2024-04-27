#include <iostream>
#include <vector>
#include <array>
#include <string>
#include <memory>
#include <algorithm>
#include <numeric>

int main(){
    std::unique_ptr<int> intpointer = std::make_unique<int>();
    std::unique_ptr<std::vector<int>> pointer = std::make_unique<std::vector<int>>(10,1);
    std::for_each(pointer->begin(), pointer->end(), [](const int &element){std::cout << element << " ";});
    std::cout << "\n";
    pointer->resize(50);
    std::iota(pointer->begin(), pointer->end(), 1);
    std::for_each(pointer->begin(), pointer->end(), [](const int &element){std::cout << element << " ";});
    std::cout << "\n";
    std::unique_ptr<char[]> cpointer = std::make_unique<char[]>(5);
    cpointer[0] = 'A';
    cpointer[1] = 'B';
    cpointer[2] = 'C';
    cpointer[3] = 'D';
    cpointer[4] = 'E';
    for(int i = 0; i < 5; ++i){
        std::cout << cpointer[i] << " ";
    }
    std::cout << "\n";
    return 0;
}
