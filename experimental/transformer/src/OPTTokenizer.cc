#include "OPTTokenizer.h"

std::vector<int> OPT_tokenize(const OPT_vocab & vocab, const std::string & text, bool add_bos) {
    std::vector<int> res(text.size() + (int) add_bos);
    return res; 
}

/*
// Defining constant map of vocab file names
std::map<std::string, std::string> VOCAB_FILES_NAMES = {
    {"vocab_file", "vocab.json"},
    {"merges_file", "merges.txt"}
};

// Defining constant maps for pretrained vocab files
std::map<std::string, std::string> vocab_file = {
    {"gpt2", "https://huggingface.co/gpt2/resolve/main/vocab.json"},
    {"gpt2-medium", "https://huggingface.co/gpt2-medium/resolve/main/vocab.json"},
    {"gpt2-large", "https://huggingface.co/gpt2-large/resolve/main/vocab.json"},
    {"gpt2-xl", "https://huggingface.co/gpt2-xl/resolve/main/vocab.json"},
    {"distilgpt2", "https://huggingface.co/distilgpt2/resolve/main/vocab.json"}
};

std::map<std::string, std::string> merges_file = {
    {"gpt2", "https://huggingface.co/gpt2/resolve/main/merges.txt"},
    {"gpt2-medium", "https://huggingface.co/gpt2-medium/resolve/main/merges.txt"},
    {"gpt2-large", "https://huggingface.co/gpt2-large/resolve/main/merges.txt"},
    {"gpt2-xl", "https://huggingface.co/gpt2-xl/resolve/main/merges.txt"},
    {"distilgpt2", "https://huggingface.co/distilgpt2/resolve/main/merges.txt"}
};

std::map<std::string, std::map<std::string, std::string>> PRETRAINED_VOCAB_FILES_MAP = {
    {"vocab_file", vocab_file},
    {"merges_file", merges_file}
};

// Defining constant map for pretrained positional embeddings sizes
std::map<std::string, int> PRETRAINED_POSITIONAL_EMBEDDINGS_SIZES = {
    {"gpt2", 1024},
    {"gpt2-medium", 1024},
    {"gpt2-large", 1024},
    {"gpt2-xl", 1024},
    {"distilgpt2", 1024}
};
*/


/* 
 *  Tokenizer 
 */
/*
class Encoder {
public:
    Encoder(std::map<std::string, int> encoder, std::vector<std::string> bpe_merges, std::string errors = "replace")
    : errors(errors) {
        this->encoder = encoder;
        for(auto &it: encoder)
            this->decoder[it.second] = it.first;
        this->byte_encoder = bytes_to_unicode();
        for(auto &it: byte_encoder)
            this->byte_decoder[it.second] = it.first;
        for(int i = 0; i < bpe_merges.size(); ++i)
            this->bpe_ranks[bpe_merges[i]] = i;
    }

    std::map<int, char> bytes_to_unicode() {
        std::vector<int> bs;
        std::vector<int> cs;
        int n = 0;

        for (int i = int('!'); i <= int('~'); ++i) bs.push_back(i);
        for (int i = int('¡'); i <= int('¬'); ++i) bs.push_back(i);
        for (int i = int('®'); i <= int('ÿ'); ++i) bs.push_back(i);

        cs = bs;  // Copy bs to cs

        for (int b = 0; b < 256; ++b) {
            if (std::find(bs.begin(), bs.end(), b) == bs.end()) {
                bs.push_back(b);
                cs.push_back(256 + n);
                ++n;
            }
        }

        for (int& n : cs) n = char(n);  // Convert int to char

        std::map<int, char> b2u;
        for (int i = 0; i < bs.size(); ++i) {
            b2u[bs[i]] = cs[i];
        }

        return b2u;
    }

    std::set<std::pair<std::string, std::string>> get_pairs(std::string word){
        std::set<std::pair<std::string, std::string>> pairs;
        char prev_char = word[0];
        for (int i = 1; i < word.length(); ++i) {
            pairs.insert(std::make_pair(prev_char, word[i]));
            prev_char = word[i];
        }

        return pairs;
    }

    std::string bpe(std::string token) {
        if(this->cache.find(token) != this->cache.end())
            return this->cache[token];
        std::vector<std::string> word (token.begin(), token.end());
        std::set<std::pair<std::string, std::string>> pairs = get_pairs(token);

        if(pairs.empty())
            return token;

        while(true) {
            std::pair<std::string, std::string> bigram = *pairs.begin();
            for(auto &pair: pairs)
                if(this->bpe_ranks[pair] < this->bpe_ranks[bigram])
                    bigram = pair;
            if(this->bpe_ranks.find(bigram) == this->bpe_ranks.end())
                break;

            std::string first = bigram.first;
            std::string second = bigram.second;
            std::vector<std::string> new_word;
            int i = 0;
            while(i < word.size()) {
                auto j = std::find(word.begin() + i, word.end(), std::string(1, first));
                if(j == word.end()) {
                    new_word.insert(new_word.end(), word.begin() + i, word.end());
                    break;
                }
                new_word.insert(new_word.end(), word.begin() + i, j);
                i = std::distance(word.begin(), j);

                if(word[i] == std::string(1, first) && i < word.size() - 1 && word[i + 1] == std::string(1, second)) {
                    new_word.push_back(std::string(1, first) + second);
                    i += 2;
                } else {
                    new_word.push_back(word[i]);
                    i += 1;
                }
            }
            word = new_word;

            if (word.size() == 1)
                break;
            else
                pairs = get_pairs(std::string(word.begin(), word.end()));
        }

        token = std::accumulate(word.begin(), word.end(), std::string(), [](std::string &s1, std::string &s2) {
            return s1 + ' ' + s2;
        });
        this->cache[token] = token;

        return token;
    }

    std::vector<int> encode(std::string text) {
        std::vector<int> bpe_tokens;

        // Using Boost Regex to tokenize
        boost::regex pat(R"('s|'t|'re|'ve|'m|'ll|'d| ?\p{L}+| ?\p{N}+| ?[^\s\p{L}\p{N}]+|\s+(?!\S)|\s+)");
        boost::sregex_iterator it(text.begin(), text.end(), pat);
        boost::sregex_iterator end;

        for (;it != end; ++it) {
            std::string token = it->str();

            std::string utf8_token;
            for (unsigned char c : token) {
                utf8_token += byte_encoder[c];
            }

            std::string bpe_result = bpe(utf8_token);
            std::istringstream iss(bpe_result);
            std::vector<std::string> results((std::istream_iterator<std::string>(iss)),
                                            std::istream_iterator<std::string>());
            for (const std::string& bpe_token : results) {
                bpe_tokens.push_back(encoder[bpe_token]);
            }
        }

        return bpe_tokens;
    }

    std::string decode(std::vector<int> tokens) {
        std::string text;
        for (int &token : tokens) {
            if (this->decoder.find(token) != this->decoder.end())
                text += this->byte_decoder[this->decoder[token][0]];
        }
        std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> converter;
        std::string converted = converter.to_bytes(text);
        return converted;
    }

private:
    std::map<std::string, int> encoder;
    std::map<int, std::string> decoder;
    std::string errors;
    std::map<int, char> byte_encoder;
    std::map<char, int> byte_decoder;
    std::map<std::pair<char, char>, int> bpe_ranks;
    std::map<std::string, std::string> cache;
};

Encoder get_encoder(std::string vocab_file, std::string bpe_file) {
    std::ifstream i(vocab_file);
    nlohmann::json j;
    i >> j;

    std::map<std::string, int> encoder = j.get<std::map<std::string, int>>();

    std::ifstream infile(bpe_file);
    std::string line;
    std::vector<std::pair<std::string, std::string>> bpe_merges;
    while (std::getline(infile, line)) {
        std::istringstream iss(line);
        std::string a, b;
        if (!(iss >> a >> b)) { break; } // error
        bpe_merges.push_back({a, b});
    }

    return Encoder(encoder, bpe_merges);
}
*/