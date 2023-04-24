#include <cmath>

#include "../operators.h"

void W8A8B8O8LinearReLU(struct W8A8B8O8Linear_params &op_params){
    Matrix3D<int8_t> x = op_params.x;
    Matrix3D<int8_t> weight = op_params.weight;
    Matrix3D<int32_t> bias = op_params.bias;
    Matrix3D<int8_t> output = op_params.output;
    float alpha = op_params.alpha;
    
    assert(output.m_dim_x == x.m_dim_x);
    assert(output.m_dim_y == x.m_dim_y);
    assert(output.m_dim_z == weight.m_dim_y);
    assert(x.m_dim_z == weight.m_dim_z);
    assert(output.m_dim_z == bias.m_dim_z);

    const int m = x.m_dim_y, k = x.m_dim_z, n = weight.m_dim_y;

    struct matmul_params params;

    params.A.row = m;
    params.A.column = k;
    params.A.int8_data_ptr = x.m_data;
    params.A.qparams.scale = alpha; // effective_scale = a * B / C
    params.B.qparams.scale = 1.0;
    params.C.qparams.scale = 1.0;
    params.A.qparams.zero_point = 0;
    params.B.row = k;
    params.B.column = n;
    params.B.int8_data_ptr = weight.m_data;
    params.B.qparams.zero_point = 0;
    params.C.row = m;
    params.C.column = n;
    params.C.int8_data_ptr = output.m_data;
    params.C.qparams.zero_point = 0;
    params.opt_params.blk_size = BLK_SIZE;
    params.opt_params.num_thread = NUM_THREAD;
    params.C.qparams.q_max = 127;
    params.C.qparams.q_min = 0;
    params.bias.int32_data_ptr = bias.m_data;
    params.bias.row = 1;
    params.bias.column = bias.m_dim_z;

    matmul::MatmulOperator matmul_op = matmul::MatmulOperator();

    // process each batch
    for (int bz = 0; bz < x.m_dim_x; bz++){
        matmul_op.mat_mul_avx_int8_fast_2x2_32unroll(&params);
        params.A.int8_data_ptr += m * k;
        params.C.int8_data_ptr += m * n;
    }

}