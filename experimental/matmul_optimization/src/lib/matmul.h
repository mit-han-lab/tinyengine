#include <sys/time.h>
// Data structures
struct quantization_params {
    float scale;
    bool per_channel = false;
    int32_t zero_point;
};

struct matrix {
    int row;
    int column;
    float *data_ptr;
    int8_t *int8_data_ptr;
    struct quantization_params qparams;
};

struct thread_args {
    const struct matrix *A;
    const struct matrix *B;
    const struct matrix *C;
    int start_i, end_i, blk_size;
};

struct optimization_params {
    int blk_size;
    int num_thread = 8;
};

struct matmul_params {
    struct matrix A, B, C;
    struct optimization_params opt_params;
};

#define MAX(A, B) ((A) > (B) ? (A) : (B))
#define MIN(A, B) ((A) < (B) ? (A) : (B))
namespace matmul {
class MatmulOperator {
   public:
    enum IMP_TYPE {
        NAIVE = 0,
        UNROLL = 1,
        REORDER = 2,
        TILING = 3,
        MULTITHREAD = 4,
        TRANSPOSE = 5,
        TRANSPOSE_SIMD = 6,
        FAST = 7,
        CUDA = 8,
        ONEDNN_FP32 = 9,
        INT8_BASELINE = 10,
        ONEDNN_INT8 = 11,
    };
    void naive_mat_mul(const struct matmul_params *params);
    void mat_mul_unrolling(const struct matmul_params *params);
    void mat_mul_reordering(const struct matmul_params *params);
    void mat_mul_tiling(const struct matmul_params *params);
    void mat_mul_multithreading(const struct matmul_params *params);
    void mat_mul_transpose(const struct matmul_params *params);
    void mat_mul_transpose_simd(const struct matmul_params *params);
    void mat_mul_fast(const struct matmul_params *params);
    void mat_mul_onednn(const struct matmul_params *params);
    void mat_mul_onednn_int8(const struct matmul_params *params);
    void naive_mat_mul_int8(const struct matmul_params *params);
    void mat_mul_cuda(const struct matmul_params *params);
    void evaluate(IMP_TYPE type, const struct matmul_params *params);

   private:
    float interval_to_us(struct timeval *start, struct timeval *end);
    void CHECK_MATRICES(const struct matrix *A, const struct matrix *B, const struct matrix *C);
};
}  // namespace matmul