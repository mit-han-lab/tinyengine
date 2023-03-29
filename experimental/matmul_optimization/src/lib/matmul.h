// Data structures
struct matrix
{
    int row;
    int column;
    float *data_ptr;
};

struct thread_args
{
    const struct matrix *A;
    const struct matrix *B;
    const struct matrix *C;
    int start_i, end_i, blk_size;
};

struct optimization_params
{
    int blk_size;
    int num_thread = 8;
};

struct matmul_params
{
    struct matrix A, B, C;
    struct optimization_params opt_params;
};

namespace matmul
{
    class MatmulOperator
    {
    public:
        enum IMP_TYPE
        {
            NAIVE = 0,
            UNROLL = 1,
            REORDER = 2,
            TILING = 3,
            MULTITHREAD = 4,
            TRANSPOSE = 5,
            TRANSPOSE_SIMD = 6,
            FAST = 7,
	    CUDA = 8
        };
        void naive_mat_mul(const struct matmul_params *params);
        void mat_mul_unrolling(const struct matmul_params *params);
        void mat_mul_reordering(const struct matmul_params *params);
        void mat_mul_tiling(const struct matmul_params *params);
        void mat_mul_multithreading(const struct matmul_params *params);
        void mat_mul_transpose(const struct matmul_params *params);
        void mat_mul_transpose_simd(const struct matmul_params *params);
        void mat_mul_fast(const struct matmul_params *params);
	void mat_mul_cuda(const struct matmul_params *params);
        void evaluate(IMP_TYPE type, const struct matmul_params *params);
    };
}
