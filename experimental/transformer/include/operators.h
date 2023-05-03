#ifndef OPERATORS_H
#define OPERATORS_H
#include <cassert>

#include "common.h"
#include "lib/matmul.h"

#define BLK_SIZE 16
#define NUM_THREAD 4

// TODO: assign these based on models
#define MAX_SQLLEN 512
#define EMBED_DIM 768
#define HIDDEN_DIM 3072

struct LayerNormQ_params {
    Matrix3D<float> weight;
    Matrix3D<float> bias;
};

class LayerNormQ {
public:
    LayerNormQ(LayerNormQ_params &params_): params(params_) {};
    LayerNormQ(){};
    void forward(const Matrix3D<float> &x, Matrix3D<int8_t> &output);
    struct LayerNormQ_params params;
};


struct W8A8B8O8Linear_params {
    Matrix3D<int8_t> weight;
    Matrix3D<int32_t> bias;
    float alpha;
    float beta;
};

class W8A8B8O8LinearReLU{
public:
    W8A8B8O8LinearReLU(W8A8B8O8Linear_params &params_);
    W8A8B8O8LinearReLU(){};
    void forward(const Matrix3D<int8_t> &x, Matrix3D<int8_t> &output);
    struct matmul_params params;
    float alpha;
    float beta;
};

class W8A8B8O8Linear{
public:
    W8A8B8O8Linear(W8A8B8O8Linear_params &params_);
    W8A8B8O8Linear(){};
    void forward(const Matrix3D<int8_t> &x, Matrix3D<int8_t> &output);
    struct matmul_params params;
    float alpha;
    float beta;
};


struct W8A8BFP32OFP32Linear_params {
    Matrix3D<int8_t> weight;
    Matrix3D<float> bias;
    float alpha;
};


class W8A8BFP32OFP32Linear{
public:
    W8A8BFP32OFP32Linear(W8A8BFP32OFP32Linear_params &params_);
    W8A8BFP32OFP32Linear(){};
    void forward(const Matrix3D<int8_t> &x, Matrix3D<float> &output);
    struct matmul_params params;
    float alpha;
};

struct BMM_S8T_S8N_S8T_params {
    float alpha;
};

class BMM_S8T_S8N_S8T{
public:
    BMM_S8T_S8N_S8T(BMM_S8T_S8N_S8T_params &params_);
    BMM_S8T_S8N_S8T(){};
    void forward(const Matrix3D<int8_t> &x, const Matrix3D<int8_t> &weight, Matrix3D<int8_t> &output);
    struct matmul_params params;
    float alpha;
};

struct BMM_S8T_S8N_F32T_params {
    float alpha;
};

class BMM_S8T_S8N_F32T{
public:
    BMM_S8T_S8N_F32T(BMM_S8T_S8N_F32T_params &params_);
    BMM_S8T_S8N_F32T(){};
    void forward(const Matrix3D<int8_t> &x, const Matrix3D<int8_t> &weight, Matrix3D<float> &output);
    struct matmul_params params;
    float alpha;
};


void load_LayerNormQ(LayerNormQ &op, std::string prefix);
void load_BMM_S8T_S8N_F32T(BMM_S8T_S8N_F32T &op, std::string prefix);
void load_BMM_S8T_S8N_S8T(BMM_S8T_S8N_S8T &op, std::string prefix);
void load_W8A8B8O8Linear_params(W8A8B8O8Linear &op, std::string prefix);
void load_W8A8B8O8LinearReLU_params(W8A8B8O8LinearReLU &op, std::string prefix);
void load_W8A8BFP32OFP32Linear_params(W8A8BFP32OFP32Linear &op, std::string prefix);

void softmax(const Matrix3D<float> &input, Matrix3D<float> &output, int dim);
void batch_Add(const Matrix3D<float> &input, const Matrix3D<float> &input2, Matrix3D<float> &output);
template <typename T>
void linear(Matrix3D<T> &a, Matrix3D<T> &b, Matrix3D<T> &c);
void linear_fp(Matrix3D<float> &a, Matrix3D<float> &b, Matrix3D<float> &c);

#endif  // OPERATORS_H