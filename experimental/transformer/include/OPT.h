#include <algorithm>
#include <cassert>
#include <cstdio>
#include <iostream>
#include <random>
#include <string>
#include <unordered_map>
#include <vector>

//
// CLI argument parsing
//
// int32_t get_num_physical_cores();

static bool OPT_eval_internal(const int* tokens, const int n_tokens, const int n_past, const int n_threads);

int OPT_eval(const int* tokens, int n_tokens, int n_past, int n_threads);

struct OPT_vocab {
    using id = int32_t;
    using token = std::string;

    struct token_score {
        token tok;
        float score;
    };

    std::unordered_map<token, id> token_to_id;
    std::vector<token_score> id_to_token;
};

typedef struct OPT_token_data {
    int id;       // token id
    float logit;  // log-odds of the token
    float p;      // probability of the token
} OPT_token_data;

typedef struct OPT_token_data_array {
    OPT_token_data* data;
    size_t size;
    bool sorted;
} OPT_token_data_array;

// inline float * OPT_get_logits(std::vector<float> logits) {
//     return logits.data();
// }

inline int OPT_n_vocab(const OPT_vocab& vocab) { return vocab.id_to_token.size(); }

struct opt_params {
    int32_t seed = -1;  // RNG seed
    // int32_t n_threads     = get_num_physical_cores(); // TODO: fix this
    int32_t n_threads = 1;
    int32_t n_predict = -1;  // new tokens to predict
    int32_t n_parts = -1;    // amount of model parts (-1 = determine from model dimensions)
    int32_t n_ctx = 512;     // context size
    int32_t n_batch = 512;   // batch size for prompt processing (must be >=32 to use BLAS)
    int32_t n_keep = 0;      // number of tokens to keep from initial prompt

    // sampling parameters
    std::unordered_map<int, float> logit_bias;  // logit bias for specific tokens
    int32_t top_k = 40;                         // <= 0 to use vocab size
    float top_p = 0.95f;                        // 1.0 = disabled
    float tfs_z = 1.00f;                        // 1.0 = disabled
    float typical_p = 1.00f;                    // 1.0 = disabled
    float temp = 0.80f;                         // 1.0 = disabled
    float repeat_penalty = 1.10f;               // 1.0 = disabled
    int32_t repeat_last_n = 64;                 // last n tokens to penalize (0 = disable penalty, -1 = context size)
    float frequency_penalty = 0.00f;            // 0.0 = disabled
    float presence_penalty = 0.00f;             // 0.0 = disabled
    int mirostat = 0;                           // 0 = disabled, 1 = mirostat, 2 = mirostat 2.0
    float mirostat_tau = 5.00f;                 // target entropy
    float mirostat_eta = 0.10f;                 // learning rate

    std::string model = "models/lamma-7B/ggml-model.bin";  // model path
    std::string prompt = "";
    std::string path_prompt_cache = "";   // path to file for saving/loading prompt eval state
    std::string input_prefix = "";        // string to prefix user inputs with
    std::string input_suffix = "";        // string to suffix user inputs with
    std::vector<std::string> antiprompt;  // string upon seeing which more user input is prompted

    std::string lora_adapter = "";  // lora adapter path
    std::string lora_base = "";     // base model path for the lora adapter

    bool memory_f16 = true;         // use f16 instead of f32 for memory kv
    bool random_prompt = false;     // do not randomize prompt if none provided
    bool use_color = false;         // use color to distinguish generations and inputs
    bool interactive = false;       // interactive mode
    bool prompt_cache_all = false;  // save user input and generations to prompt cache

    bool embedding = false;          // get only sentence embedding
    bool interactive_first = false;  // wait for user input immediately
    bool multiline_input = false;    // reverse the usage of `\`

    bool instruct = false;        // instruction mode (used for Alpaca models)
    bool penalize_nl = true;      // consider newlines as a repeatable token
    bool perplexity = false;      // compute perplexity over the prompt
    bool use_mmap = true;         // use mmap for faster loads
    bool use_mlock = false;       // use mlock to keep model in memory
    bool mem_test = false;        // compute maximum memory usage
    bool verbose_prompt = false;  // print prompt tokens before generation
};

bool opt_params_parse(int argc, char** argv, opt_params& params);

void OPT_sample_repetition_penalty(OPT_token_data_array* candidates, const int* last_tokens, size_t last_tokens_size,
                                   float penalty);

void OPT_sample_frequency_and_presence_penalties(OPT_token_data_array* candidates, const int* last_tokens_p,
                                                 size_t last_tokens_size, float alpha_frequency, float alpha_presence);

int OPT_sample_token_greedy(OPT_token_data_array* candidates);

void OPT_sample_temperature(OPT_token_data_array* candidates_p, float temp);

void OPT_sample_softmax(OPT_token_data_array* candidates);

int OPT_sample_token(OPT_token_data_array* candidates);

void OPT_sample_top_k(OPT_token_data_array* candidates, int k, size_t min_keep);

int OPT_sample_token_mirostat(const OPT_vocab& vocab, OPT_token_data_array* candidates, float tau, float eta, int m,
                              float* mu);

int OPT_sample_token_mirostat_v2(OPT_token_data_array* candidates, float tau, float eta, float* mu);

void OPT_sample_tail_free(OPT_token_data_array* candidates, float z, size_t min_keep);

void OPT_sample_typical(OPT_token_data_array* candidates, float p, size_t min_keep);

void OPT_sample_top_p(OPT_token_data_array* candidates, float p, size_t min_keep);
