#ifndef UTILS_H
#define UTILS_H

#include <fstream>
#include <cstdlib>
#include <math.h>
#include <typeinfo>

#define ERROR_MAX 0.01
#define INT_ERROR_MAX 1

template<typename T>
void read_to_array(char* path, T* array, int size);

template<typename T>
bool check_two_equal(T* array, T* array2, int size);

template<>
bool check_two_equal(int8_t* array, int8_t* array2, int size);

#endif