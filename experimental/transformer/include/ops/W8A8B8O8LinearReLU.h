#include "common.h"

class W8A8B8O8LinearReLU{
public:
    W8A8B8O8LinearReLU(W8A8B8O8Linear_params &params_);
    W8A8B8O8LinearReLU(){};
    void forward(const Matrix3D<int8_t> &x, Matrix3D<int8_t> &output);
    struct matmul_params params;
    float alpha;
    float beta;
};

void load_W8A8B8O8LinearReLU_params(W8A8B8O8LinearReLU &op, std::string prefix);