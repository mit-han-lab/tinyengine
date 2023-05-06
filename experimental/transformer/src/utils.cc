#include "utils.h"
#include <stdio.h>
#include <cstdlib>
#include <iostream>
#include <cassert>

template<typename T>
void read_to_array(const char* path, T* array, int size){
    std::ifstream infile(path, std::ios::binary | std::ios::in);
    assert(infile);
    infile.read(reinterpret_cast<char*>(array), size * sizeof(T));
    infile.close();
}

bool check_two_equal(float* array, float* array2, int size, float error){
    float sq_diff = 0;
    for (int i = 0; i < size; i++){
        float diff = (float)array[i] - (float)array2[i];
        sq_diff += diff * diff;
    }
    if ((sq_diff / size) > error){
        std::cout << "MSE:" << sq_diff/size << std::endl;
        return false;
    }
    return true;
}

template<typename T>
bool check_two_equal(T* array, T* array2, int size){
    float sq_diff = 0;
    for (int i = 0; i < size; i++){
        float diff = (float)array[i] - (float)array2[i];
        sq_diff += diff * diff;
    }
    if ((sq_diff / size) > ERROR_MAX){
        std::cout << "MSE:" << sq_diff/size << std::endl;
        return false;
    }
    return true;
}

template<>
bool check_two_equal<int8_t>(int8_t* array, int8_t* array2, int size){
    float sq_diff = 0;
    for (int i = 0; i < size; i++){
        float diff = (float)array[i] - (float)array2[i];
        sq_diff += diff * diff;
    }
    if ((sq_diff / size) > INT_ERROR_MAX){
        std::cout << "MSE:" << sq_diff/size << std::endl;
        return false;
    }
    return true;
}

bool check_two_equal(int8_t* array, int8_t* array2, int size, float error){
    float sq_diff = 0;
    for (int i = 0; i < size; i++){
        float diff = (float)array[i] - (float)array2[i];
        sq_diff += diff * diff;
    }
    if ((sq_diff / size) > error){
        std::cout << "MSE:" << sq_diff/size << std::endl;
        return false;
    }
    return true;
}

template<>
bool check_two_equal<int>(int* array, int* array2, int size){
    float sq_diff = 0;
    for (int i = 0; i < size; i++){
        float diff = (float)array[i] - (float)array2[i];
        sq_diff += diff * diff;
    }
    if ((sq_diff / size) > INT_ERROR_MAX){
        std::cout << "MSE:" << sq_diff/size << std::endl;
        return false;
    }
    return true;
}



void print_first_k_elelment(std::string name, const int8_t *arr, int k, int start_idx){
    std::cout << name << ":";
    for (int i = start_idx; i < k; i++){
        std::cout << static_cast<int>(arr[i]) << ",";
    }
    std::cout << std::endl;
}

void print_first_k_elelment(std::string name, const int32_t *arr, int k, int start_idx){
    std::cout << name << ":";
    for (int i = start_idx; i < k; i++){
        std::cout << static_cast<int>(arr[i]) << ",";
    }
    std::cout << std::endl;
}


void print_first_k_elelment(std::string name, const float *arr, int k, int start_idx){
    std::cout << name << ":";
    for (int i = start_idx; i < k; i++){
        std::cout << static_cast<float>(arr[i]) << ",";
    }
    std::cout << std::endl;
}


// Explicitly instantiate the generic template function for other types (if needed)
template bool check_two_equal<float>(float* array, float* array2, int size);
template void read_to_array<float>(const char* path, float* array, int size);
template void read_to_array<int8_t>(const char* path, int8_t* array, int size);
template void read_to_array<int32_t>(const char* path, int32_t* array, int size);