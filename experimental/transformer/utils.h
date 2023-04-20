#ifndef UTILS_H
#define UTILS_H

#include <fstream>
#include <cstdlib>

template<typename T>
void read_to_array(char* path, T* array, int size){
    std::ifstream infile(path, std::ios::binary | std::ios::in);
    infile.read(reinterpret_cast<char*>(array), size * sizeof(T));
    infile.close();
}

#endif