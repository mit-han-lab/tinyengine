#include "utils.h"
#include <stdio.h>
#include <cstdlib>
#include <iostream>

template<typename T>
void read_to_array(char* path, T* array, int size){
    std::ifstream infile(path, std::ios::binary | std::ios::in);
    infile.read(reinterpret_cast<char*>(array), size * sizeof(T));
    infile.close();
}

template<typename T>
bool check_two_equal(T* array, T* array2, int size){
    for (int i = 0; i < size; i++){
        float diff = (float)array[i] - (float)array2[i];
        if (abs(diff) > ERROR_MAX)
            return false;
    }
    return true;
}

template<>
bool check_two_equal<int8_t>(int8_t* array, int8_t* array2, int size){
    for (int i = 0; i < size; i++){
        int diff = (int)array[i] - (int)array2[i];
        // std::cout << "@@@@" << std::endl;
        if (abs(diff) > INT_ERROR_MAX){
            std::cout << i << ":" << diff << std::endl;
            return false;
        }
    }
    return true;
}

// Explicitly instantiate the generic template function for other types (if needed)
template bool check_two_equal<float>(float* array, float* array2, int size);
template void read_to_array<float>(char* path, float* array, int size);
template void read_to_array<int8_t>(char* path, int8_t* array, int size);