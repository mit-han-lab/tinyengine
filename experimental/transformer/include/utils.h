#ifndef UTILS_H
#define UTILS_H

#include <fstream>
#include <cstdlib>
#include <math.h>
#include <typeinfo>

#define ERROR_MAX 0.0002
#define INT_ERROR_MAX 1

template<typename T>
void read_to_array(const char* path, T* array, int size);

template<typename T>
bool check_two_equal(T* array, T* array2, int size);

template<>
bool check_two_equal(int8_t* array, int8_t* array2, int size);

bool check_two_equal(int8_t* array, int8_t* array2, int size, float error);

bool check_two_equal(float* array, float* array2, int size, float error);


void print_first_k_elelment(std::string name, const int8_t * arr, int k, int start_idx = 0);
void print_first_k_elelment(std::string name, const int32_t * arr, int k, int start_idx = 0);
void print_first_k_elelment(std::string name, const float * arr, int k, int start_idx = 0);

#endif