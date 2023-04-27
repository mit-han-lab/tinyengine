#include "utils.h"
#include <stdio.h>
#include <cstdlib>
#include <iostream>

template<typename T>
void read_to_array(const char* path, T* array, int size){
    std::ifstream infile(path, std::ios::binary | std::ios::in);
    infile.read(reinterpret_cast<char*>(array), size * sizeof(T));
    infile.close();
}

template<typename T>
bool check_two_equal(T* array, T* array2, int size){
    for (int i = 0; i < size; i++){
        float diff = (float)array[i] - (float)array2[i];
        if (abs(diff) > ERROR_MAX){
            std::cout << i << ":" << diff << "," << (float)array[i] << "," << (float)array2[i] << std::endl;
            return false;
        }
    }
    return true;
}

template<>
bool check_two_equal<int8_t>(int8_t* array, int8_t* array2, int size){
    for (int i = 0; i < size; i++){
        int diff = (int)array[i] - (int)array2[i];
        if (abs(diff) > INT_ERROR_MAX){
            std::cout << i << ":" << diff << "," << (int)array[i] << "," << (int)array2[i] << std::endl;
            return false;
        }
    }
    return true;
}


void print_first_k_elelment(std::string name, const int8_t *arr, int k){
    std::cout << name << ":";
    for (int i = 0; i < k; i++){
        std::cout << static_cast<int>(arr[i]) << ",";
    }
    std::cout << std::endl;
}

// Explicitly instantiate the generic template function for other types (if needed)
template bool check_two_equal<float>(float* array, float* array2, int size);
template void read_to_array<float>(const char* path, float* array, int size);
template void read_to_array<int8_t>(const char* path, int8_t* array, int size);
template void read_to_array<int32_t>(const char* path, int32_t* array, int size);