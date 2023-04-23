#include <cmath>

#include "../operators.h"

void LayerNormQ(Matrix3D<float> &x, Matrix3D<float> &weight, Matrix3D<float> &bias, Matrix3D<int8_t> &output,
                int last_dims, float eps) {
    assert(last_dims == 2);  // support the last dim for now
    assert(output.m_dim_x == x.m_dim_x);
    assert(output.m_dim_y == x.m_dim_y);
    assert(output.m_dim_z == x.m_dim_z);
    assert(x.m_dim_z == weight.m_dim_z);
    assert(x.m_dim_z == bias.m_dim_z);

    for (int i = 0; i < x.m_dim_x; i++) {      // batches
        for (int j = 0; j < x.m_dim_y; j++) {  // samples
            float mean = 0;
            for (int k = 0; k < x.m_dim_z; k++) {  // hideden states
                mean += x(i, j, k);
            }
            mean /= x.m_dim_z;
            float squared_diff_sum = 0;
            for (int k = 0; k < x.m_dim_z; k++) {
                float value = x(i, j, k);
                squared_diff_sum += (value - mean) * (value - mean);
            }
            float std_dev = std::sqrt(squared_diff_sum / x.m_dim_z);

            for (int k = 0; k < x.m_dim_z; k++) {
                float value = x(i, j, k);
                float fp_out = ((value - mean) / (std_dev + eps) * weight(0, 0, k)) + bias(0, 0, k);
                output(i, j, k) = static_cast<int8_t>(fp_out);
            }
        }
    }
}