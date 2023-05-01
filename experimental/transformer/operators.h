#ifndef OPERATORS_H
#define OPERATORS_H
#include <cassert>

#include "common.h"
#include "lib/matmul.h"

#define BLK_SIZE 16
#define NUM_THREAD 4

#define MAX_SQLLEN 512
#define EMBED_DIM 768

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
    float beta;
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

void load_LayerNormQ(struct LayerNormQ_params &param, std::string prefix);
void load_BMM_S8T_S8N_F32T(struct BMM_S8T_S8N_F32T_params &param, std::string prefix);
void load_BMM_S8T_S8N_S8T(struct BMM_S8T_S8N_S8T_params &param, std::string prefix);
void load_W8A8B8O8Linear_params(struct W8A8B8O8Linear_params &param, std::string prefix);
void load_W8A8BFP32OFP32Linear_params(struct W8A8BFP32OFP32Linear_params &param, std::string prefix);

void LayerNormQ(LayerNormQ_params &op_params);
void W8A8B8O8LinearReLU(struct W8A8B8O8Linear_params &op_params);
void W8A8B8O8Linear(struct W8A8B8O8Linear_params &op_params);
void W8A8BFP32OFP32Linear(struct W8A8BFP32OFP32Linear_params &op_params);
void BMM_S8T_S8N_F32T(struct BMM_S8T_S8N_F32T_params &op_params);
void BMM_S8T_S8N_S8T(struct BMM_S8T_S8N_S8T_params &op_params);
void softmax(const Matrix3D<float> &input, Matrix3D<float> &output, int dim);
void batch_Add(const Matrix3D<float> &input, const Matrix3D<float> &input2, Matrix3D<float> &output);

template <typename T>
void linear(Matrix3D<T> &a, Matrix3D<T> &b, Matrix3D<T> &c);
void linear_fp(Matrix3D<float> &a, Matrix3D<float> &b, Matrix3D<float> &c);

#endif  // OPERATORS_H