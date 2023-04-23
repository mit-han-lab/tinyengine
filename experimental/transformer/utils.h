#ifndef UTILS_H
#define UTILS_H

#include <fstream>
#include <cstdlib>
#include <math.h>

#define ERROR_MAX 0.01

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

#endif