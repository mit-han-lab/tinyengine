#include <cstdio>
#include <vector>
#include <string>
#include <queue>
#include <cassert>
#include <unordered_map>
#include <random>

#include "OPTTokenizer.h"

void OPTGenerate(std::vector<int> embd_inp, 
                       const bool is_interacting, 
          const struct opt_params params, 
         const std::vector<float> _logits,
                const OPT_vocab & vocab);
