#include <cmath>

#include "../operators.h"

W8A8B8O8Linear::W8A8B8O8Linear(struct W8A8B8O8Linear_params &op_params){
    Matrix3D<int8_t> weight = op_params.weight;
    Matrix3D<int32_t> bias = op_params.bias;
    alpha = op_params.alpha;

    int k = weight.m_dim_z, n = weight.m_dim_y;
    params.A.qparams.scale = alpha; // effective_scale = a * B / C
    params.B.qparams.scale = 1.0;
    params.C.qparams.scale = 1.0;
    params.A.qparams.zero_point = 0;
    params.B.row = k;
    params.B.column = n;
    params.B.int8_data_ptr = weight.m_data;
    params.B.qparams.zero_point = 0;
    params.C.qparams.zero_point = 0;
    params.opt_params.blk_size = BLK_SIZE;
    params.opt_params.num_thread = NUM_THREAD;
    params.C.qparams.q_max = 127;
    params.C.qparams.q_min = -128;
    params.bias.int32_data_ptr = bias.m_data;
    params.bias.row = 1;
    params.bias.column = n;
}

void W8A8B8O8Linear::forward(const Matrix3D<int8_t> &x, Matrix3D<int8_t> &output){
    assert(output.m_dim_x == x.m_dim_x);
    assert(output.m_dim_y == x.m_dim_y);
    assert(output.m_dim_z == params.B.column);
    assert(x.m_dim_z == params.B.row);
    assert(output.m_dim_z == params.bias.column);

    const int m = x.m_dim_y, k = x.m_dim_z, n = params.B.column;

    params.A.row = m;
    params.A.column = k;
    params.A.int8_data_ptr = x.m_data;
    params.C.row = m;
    params.C.column = n;
    params.C.int8_data_ptr = output.m_data;
    params.A.qparams.scale = alpha;

    matmul::MatmulOperator matmul_op = matmul::MatmulOperator();

    // process each batch
    for (int bz = 0; bz < x.m_dim_x; bz++){
        matmul_op.mat_mul_avx_int8_fast_2x2_32unroll(&params);
        params.A.int8_data_ptr += m * k;
        params.C.int8_data_ptr += m * n;
    }

}