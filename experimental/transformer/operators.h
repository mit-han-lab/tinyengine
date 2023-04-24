#ifndef OPERATORS_H
#define OPERATORS_H
#include <cassert>

#include "common.h"
#include "lib/matmul.h"

#define BLK_SIZE 16
#define NUM_THREAD 4

struct LayerNormQ_params {
    Matrix3D<float> x;
    Matrix3D<float> weight;
    Matrix3D<float> bias;
    Matrix3D<int8_t> output;
};

struct W8A8BFP32OFP32Linear_params {
    Matrix3D<int8_t> x;
    Matrix3D<int8_t> weight;
    Matrix3D<float> bias;
    Matrix3D<float> output;
    float alpha;
};

struct W8A8B8O8Linear_params {
    Matrix3D<int8_t> x;
    Matrix3D<int8_t> weight;
    Matrix3D<int32_t> bias;
    Matrix3D<int8_t> output;
    float alpha;
};

struct BMM_S8T_S8N_S8T_params {
    Matrix3D<int8_t> x;
    Matrix3D<int8_t> weight;
    Matrix3D<int8_t> output;
    float alpha;
};

struct BMM_S8T_S8N_F32T_params {
    Matrix3D<int8_t> x;
    Matrix3D<int8_t> weight;
    Matrix3D<float> output;
    float alpha;
};

void LayerNormQ(LayerNormQ_params &op_params);
void W8A8B8O8LinearReLU(struct W8A8B8O8Linear_params &op_params);
void W8A8B8O8Linear(struct W8A8B8O8Linear_params &op_params);
void W8A8BFP32OFP32Linear(struct W8A8BFP32OFP32Linear_params &op_params);
void BMM_S8T_S8N_F32T(struct BMM_S8T_S8N_F32T_params &op_params);
void BMM_S8T_S8N_S8T(struct BMM_S8T_S8N_S8T_params &op_params);

template <typename T>
void linear(Matrix3D<T> &a, Matrix3D<T> &b, Matrix3D<T> &c);
void linear_fp(Matrix3D<float> &a, Matrix3D<float> &b, Matrix3D<float> &c);

#endif  // OPERATORS_H