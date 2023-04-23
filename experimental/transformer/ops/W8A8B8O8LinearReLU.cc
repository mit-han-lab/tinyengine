

void W8A8B8O8LinearReLU(Matrix3D<int8_t> &x, Matrix3D<int8_t> &weight, Matrix3D<int32_t> &bias, Matrix3D<int8_t> &output, float alpha);

#include <cmath>

#include "../lib/matmul.h"
#include "../operators.h"

#define BLK_SIZE 16
#define NUM_THREAD 8

void W8A8B8O8LinearReLU(Matrix3D<int8_t> &x, Matrix3D<int8_t> &weight, Matrix3D<int32_t> &bias, Matrix3D<int8_t> &output, float alpha) {


    assert(output.m_dim_x == x.m_dim_x);
    assert(output.m_dim_y == x.m_dim_y);
    assert(output.m_dim_z == weight.m_dim_y);
    assert(x.m_dim_z == weight.m_dim_z);
    assert(x.m_dim_z == bias.m_dim_z);

    const int m = x.m_dim_y, k = x.m_dim_z, n = weight.m_dim_y;

    struct matmul_params params;

    // TODO: Bias
    params.A.row = m;
    params.A.column = k;
    params.A.int8_data_ptr = x.m_data;
    params.A.qparams.scale = alpha; // effective_scale = a * B / C
    params.B.qparams.scale = 1.0;
    params.C.qparams.scale = 1.0;
    params.A.qparams.zero_point = 0;
    params.B.row = n;
    params.B.column = k;
    params.B.int8_data_ptr = weight.m_data;
    params.B.qparams.zero_point = 0;
    params.C.row = m;
    params.C.column = n;
    params.C.int8_data_ptr = output.m_data;
    params.C.qparams.zero_point = 0;
    params.opt_params.blk_size = BLK_SIZE;
    params.opt_params.num_thread = NUM_THREAD;

    MatmulOperator matmul_op = MatmulOperator();

    // process each batch
    for (int bz = 0; bz < x.m_dim_x; bz++){
        matmul_op.mat_mul_avx_int8_thread_func_2x2_32unroll(params);
        params.A.int8_data_ptr += m * k;
        params.C.int8_data_ptr += m * n;
    }

}