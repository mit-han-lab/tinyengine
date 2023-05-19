#include <cstdio>
#include <vector>
#include <string>
#include <queue>
#include <cassert>
#include <unordered_map>
#include <random>
#include <map>
#include <set>
#include <algorithm>
#include <cmath>
#include <codecvt>
#include <locale>
#include <sstream>
#include <fstream>
#include <utility>
//#include <boost/regex.hpp>
//#include <nlohmann/json.hpp>

#include "OPT.h"

/* TODO */
inline int n_ctx = 1024;
inline std::vector<int> last_n_tokens(n_ctx);

inline int OPT_token_bos() {
    return 1;
}

inline int OPT_token_eos() {
    return 2;
}

inline int OPT_token_nl() {
    return 13;
}

std::vector<int> OPT_tokenize(const OPT_vocab & vocab, const std::string & text, bool add_bos);
