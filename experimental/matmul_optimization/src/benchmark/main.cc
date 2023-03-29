#include "lib/matmul.h"

#include <stdio.h>
#include <math.h>

#define BLK_SIZE 32
#define MAX_PRECISION_ERROR 0.01

#define A_ROW 640
#define A_COLUMN 12800
#define B_ROW 12800
#define B_COLUMN 640
#define C_ROW 640
#define C_COLUMN 640
#define NUM_THREAD 4

float MAT_A[A_ROW * A_COLUMN];
float MAT_B[B_ROW * B_COLUMN];
float transpose_B[B_ROW * B_COLUMN];
float native_C[C_ROW * C_COLUMN];
float output_C[C_ROW * C_COLUMN];

bool check_identical(float matA[], float matB[], int size)
{
    for (int i = 0; i < size; i++)
    {
        if (abs((matA[i] - matB[i]) / (matA[i])) > MAX_PRECISION_ERROR)
        {
            printf("%f, %f", matA[i], matB[i]);
            return false;
        }
    }
    return true;
}

void initialize_matrix(float A[], int size)
{
    for (int i = 0; i < size; i++)
    {
        A[i] = (float)(rand()) / (float)(RAND_MAX);
    }
}

using namespace matmul;

int main()
{
    // initialize
    initialize_matrix(MAT_A, A_ROW * A_COLUMN);
    initialize_matrix(MAT_B, B_ROW * B_COLUMN);
    initialize_matrix(native_C, C_ROW * C_COLUMN);

    MatmulOperator matmul_op = MatmulOperator();

    struct matmul_params params;
    params.A.row = A_ROW; params.A.column = A_COLUMN; params.A.data_ptr = MAT_A;
    params.B.row = B_ROW; params.B.column = B_COLUMN; params.B.data_ptr = MAT_B;
    params.C.row = C_ROW; params.C.column = C_COLUMN;
    params.opt_params.blk_size = BLK_SIZE; params.opt_params.num_thread = NUM_THREAD;
    
    //Baseline
    params.C.data_ptr = native_C;
    matmul_op.evaluate(MatmulOperator::NAIVE, &params);

    params.C.data_ptr = output_C;
    // unrolling
    matmul_op.evaluate(MatmulOperator::UNROLL, &params);
    if (!check_identical(native_C, output_C, C_ROW * C_COLUMN))
        printf("incorrect output from mat_mul_unrolling\n");

    // reordering
    matmul_op.evaluate(MatmulOperator::REORDER, &params);
    if (!check_identical(native_C, output_C, C_ROW * C_COLUMN))
        printf("incorrect output of mat_mul_reordering\n");

    // tiling
    matmul_op.evaluate(MatmulOperator::TILING, &params);
    if (!check_identical(native_C, output_C, C_ROW * C_COLUMN))
        printf("incorrect output of mat_mul_tiling\n");

    // multithreading
    matmul_op.evaluate(MatmulOperator::MULTITHREAD, &params);
    if (!check_identical(native_C, output_C, C_ROW * C_COLUMN))
        printf("incorrect output of mat_mul_multithreading\n");

    // transpose
    matmul_op.evaluate(MatmulOperator::TRANSPOSE, &params);
    if (!check_identical(native_C, output_C, C_ROW * C_COLUMN))
        printf("incorrect output of mat_mul_transpose\n");

    // transpose + simd
    matmul_op.evaluate(MatmulOperator::TRANSPOSE_SIMD, &params);
    if (!check_identical(native_C, output_C, C_ROW * C_COLUMN))
        printf("incorrect output of mat_mul_transpose_simd\n");

    // cuda
    matmul_op.evaluate(MatmulOperator::CUDA, &params);
    if (!check_identical(native_C, output_C, C_ROW * C_COLUMN))
        printf("incorrect output of mat_mul_cuda\n");

    // For fast, we need to transpose B first
    for (int i = 0; i < B_COLUMN; i++)
        for (int j = 0; j < B_ROW; j++)
            transpose_B[i * B_ROW + j] = MAT_B[j * B_COLUMN + i];
    params.B.column = B_ROW;
    params.B.row = B_COLUMN;
    params.B.data_ptr = transpose_B;
    params.opt_params.blk_size = 4;
    params.opt_params.num_thread = 4;

    // fast
    matmul_op.evaluate(MatmulOperator::FAST, &params);
    if (!check_identical(native_C, output_C, C_ROW * C_COLUMN))
        printf("incorrect output of mat_mul_fast\n");

    return 0;
}
