#ifndef OPERATORS_H
#define OPERATORS_H
#include <cassert>

#include "common.h"
#include "lib/matmul.h"

#define BLK_SIZE 16
#define NUM_THREAD 4

// TODO: assign these based on models
#define BATCH 1
#define HEAD 12
#define MAXSQLEN 512
#define EMBED_DIM 768
#define HIDDEN_DIM 3072

// include all ops
#include "ops/BMM_S8T_S8N_F32T.h"
#include "ops/BMM_S8T_S8N_S8T.h"
#include "ops/LayerNormQ.h"
#include "ops/W8A8B8O8Linear.h"
#include "ops/W8A8B8O8LinearReLU.h"
#include "ops/W8A8BFP32OFP32Linear.h"
#include "ops/Embedding.h"

void softmax(const Matrix3D<float> &input, Matrix3D<float> &output, int dim);
void batch_Add(const Matrix3D<float> &input, const Matrix3D<float> &input2, Matrix3D<float> &output);
template <typename T>
void linear(Matrix3D<T> &a, Matrix3D<T> &b, Matrix3D<T> &c);
void linear_fp(Matrix3D<float> &a, Matrix3D<float> &b, Matrix3D<float> &c);


#endif  // OPERATORS_H