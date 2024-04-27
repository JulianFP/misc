#include <iostream>
#include <vector>
#include <cstdlib>
#include <chrono>

int pickPivotMiddle(int first, int last){
    return (first + last) / 2;
}

template<class Func>
void quickSort(int *input, int first, int last, Func *pickPivot){
    if(first >= last) return;
    int i = first;
    int j = last;
    int p = pickPivot(first, last);
    while(i <= j){
        while(i <= j && input[i] < input[p]) ++i;
        while(i <= j && input[j] > input[p]) --j;
        if(i <= j){
            std::swap(input[i], input[j]);
            ++i;
            --j;
        }
    }
    quickSort(input, first, j, pickPivot);
    quickSort(input, i, last, pickPivot);
}

int main(){
    int n;
    int size;
    std::cout << "Enter number of arrays and size: " << std::flush;
    std::cin >> n;
    std::cin >> size;

    std::vector<int> runtimesMiddle;
    std::vector<int> runtimesRandom;

    for(int i = 0; i < n; ++i){
        int arr[size];
        for(int j = 0; j < size; ++j){
            arr[j] = rand();
        }
        auto Middlestart = std::chrono::high_resolution_clock::now();
        quickSort(arr, 0, size-1, pickPivotMiddle);
        auto Middleend = std::chrono::high_resolution_clock::now();
        auto durMiddle = std::chrono::duration_cast<std::chrono::microseconds>(Middleend - Middlestart);
        runtimesMiddle.push_back(durMiddle.count());
        auto Randomstart = std::chrono::high_resolution_clock::now();
        quickSort(arr, 0, size-1, pickPivotMiddle);
        auto Randomend = std::chrono::high_resolution_clock::now();
        auto durRandom = std::chrono::duration_cast<std::chrono::microseconds>(Randomend - Randomstart);
        runtimesRandom.push_back(durRandom.count());
    }
    return 0;
}
