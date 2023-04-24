#ifndef OPERATORS_H
#define OPERATORS_H
#include <cassert>
#include "common.h"
#include "lib/matmul.h"

#define BLK_SIZE 16
#define NUM_THREAD 4

void LayerNormQ(Matrix3D<float> &x, Matrix3D<float> &weight, Matrix3D<float> &bias, Matrix3D<int8_t> &output, int last_dims=2, float eps = 0.00001);
void W8A8B8O8LinearReLU(Matrix3D<int8_t> &x, Matrix3D<int8_t> &weight, Matrix3D<int32_t> &bias, Matrix3D<int8_t> &output, float alpha);
void W8A8B8O8Linear(Matrix3D<int8_t> &x, Matrix3D<int8_t> &weight, Matrix3D<int32_t> &bias, Matrix3D<int8_t> &output, float alpha);
void W8A8BFP32OFP32Linear(Matrix3D<int8_t> &x, Matrix3D<int8_t> &weight, Matrix3D<float> &bias, Matrix3D<float> &output, float alpha);

template<typename T>
void linear(Matrix3D<T> &a, Matrix3D<T> &b, Matrix3D<T> &c);
void linear_fp(Matrix3D<float> &a, Matrix3D<float> &b, Matrix3D<float> &c);

#endif //OPERATORS_H