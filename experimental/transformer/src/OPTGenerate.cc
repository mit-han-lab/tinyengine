#include "OPTGenerate.h"

void OPTGenerate(std::vector<int> embd_inp, const bool is_interacting, const struct opt_params params,
                 const std::vector<float> _logits, const OPT_vocab& vocab) {
    /* TODO */
    std::fill(last_n_tokens.begin(), last_n_tokens.end(), 0);
    /*
    // TODO: session saving
    std::vector<int> session_tokens;
    int n_session_consumed = 0;
    */

    int n_past = 0;
    int n_remain = params.n_predict;
    int n_consumed = 0;
    std::vector<int> embd;

    while (n_remain != 0) {
        // predict
        if (embd.size() > 0) {
            // infinite text generation via context swapping
            // if we run out of context:
            // - take the n_keep first tokens from the original prompt (via n_past)
            // - take half of the last (n_ctx - n_keep) tokens and recompute the logits in batches
            if (n_past + (int)embd.size() > n_ctx) {
                const int n_left = n_past - params.n_keep;

                // always keep the first token - BOS
                n_past = std::max(1, params.n_keep);

                // insert n_left/2 tokens at the start of embd from last_n_tokens
                embd.insert(embd.begin(), last_n_tokens.begin() + n_ctx - n_left / 2 - embd.size(),
                            last_n_tokens.end() - embd.size());

                // stop saving session if we run out of context
                // path_session.clear();

                // printf("\n---\n");
                // printf("resetting: '");
                // for (int i = 0; i < (int) embd.size(); i++) {
                //     printf("%s", llama_token_to_str(ctx, embd[i]));
                // }
                // printf("'\n");
                // printf("\n---\n");
            }

            /*
            // TODO: session saving
            // try to reuse a matching prefix from the loaded session instead of re-eval (via n_past)
            if (n_session_consumed < (int) session_tokens.size()) {
                size_t i = 0;
                for ( ; i < embd.size(); i++) {
                    if (embd[i] != session_tokens[n_session_consumed]) {
                        session_tokens.resize(n_session_consumed);
                        break;
                    }

                    n_past++;
                    n_session_consumed++;

                    if (n_session_consumed >= (int) session_tokens.size()) {
                        ++i;
                        break;
                    }
                }
                if (i > 0) {
                    embd.erase(embd.begin(), embd.begin() + i);
                }
            }
            */

            // evaluate tokens in batches
            // embd is typically prepared beforehand to fit within a batch, but not always
            for (int i = 0; i < (int)embd.size(); i += params.n_batch) {
                int n_eval = (int)embd.size() - i;
                if (n_eval > params.n_batch) {
                    n_eval = params.n_batch;
                }
                if (OPT_eval(&embd[i], n_eval, n_past, params.n_threads)) {
                    // fprintf(stderr, "%s : failed to eval\n", __func__);
                    fprintf(stderr, "Failed to eval in OPT_eval().\n");
                    // return 1;
                }
                n_past += n_eval;
            }

            /*
            // TODO: session saving
            if (embd.size() > 0 && !path_session.empty()) {
                session_tokens.insert(session_tokens.end(), embd.begin(), embd.end());
                n_session_consumed = session_tokens.size();
            }
            */
        }

        embd.clear();

        if ((int)embd_inp.size() <= n_consumed && !is_interacting) {
            // out of user input, sample next token
            const float temp = params.temp;
            const int32_t top_k = params.top_k <= 0 ? OPT_n_vocab(vocab) : params.top_k;
            const float top_p = params.top_p;
            const float tfs_z = params.tfs_z;
            const float typical_p = params.typical_p;
            const int32_t repeat_last_n = params.repeat_last_n < 0 ? n_ctx : params.repeat_last_n;
            const float repeat_penalty = params.repeat_penalty;
            const float alpha_presence = params.presence_penalty;
            const float alpha_frequency = params.frequency_penalty;
            const int mirostat = params.mirostat;
            const float mirostat_tau = params.mirostat_tau;
            const float mirostat_eta = params.mirostat_eta;
            const bool penalize_nl = params.penalize_nl;

            // optionally save the session on first sample (for faster prompt loading next time)
            /*
            if (!path_session.empty() && need_to_save_session) {
                need_to_save_session = false;
                llama_save_session_file(ctx, path_session.c_str(), session_tokens.data(), session_tokens.size());
            }
            */

            int id = 0;

            {
                auto internal_logits = _logits;
                auto logits = internal_logits;
                auto n_vocab = OPT_n_vocab(vocab);

                // Apply params.logit_bias map
                for (auto it = params.logit_bias.begin(); it != params.logit_bias.end(); it++) {
                    logits[it->first] += it->second;
                }

                std::vector<OPT_token_data> candidates;
                candidates.reserve(n_vocab);
                for (int token_id = 0; token_id < n_vocab; token_id++) {
                    candidates.emplace_back(OPT_token_data{token_id, logits[token_id], 0.0f});
                }

                OPT_token_data_array candidates_p = {candidates.data(), candidates.size(), false};

                // Apply penalties
                float nl_logit = logits[OPT_token_nl()];
                auto last_n_repeat = std::min(std::min((int)last_n_tokens.size(), repeat_last_n), n_ctx);
                OPT_sample_repetition_penalty(&candidates_p,
                                              last_n_tokens.data() + last_n_tokens.size() - last_n_repeat,
                                              last_n_repeat, repeat_penalty);
                OPT_sample_frequency_and_presence_penalties(&candidates_p,
                                                            last_n_tokens.data() + last_n_tokens.size() - last_n_repeat,
                                                            last_n_repeat, alpha_frequency, alpha_presence);
                if (!penalize_nl) {
                    logits[OPT_token_nl()] = nl_logit;
                }

                if (temp <= 0) {
                    // Greedy sampling
                    id = OPT_sample_token_greedy(&candidates_p);
                } else {
                    if (mirostat == 1) {
                        static float mirostat_mu = 2.0f * mirostat_tau;
                        const int mirostat_m = 100;
                        OPT_sample_temperature(&candidates_p, temp);
                        id = OPT_sample_token_mirostat(vocab, &candidates_p, mirostat_tau, mirostat_eta, mirostat_m,
                                                       &mirostat_mu);
                    } else if (mirostat == 2) {
                        static float mirostat_mu = 2.0f * mirostat_tau;
                        OPT_sample_temperature(&candidates_p, temp);
                        id = OPT_sample_token_mirostat_v2(&candidates_p, mirostat_tau, mirostat_eta, &mirostat_mu);
                    } else {
                        // Temperature sampling
                        OPT_sample_top_k(&candidates_p, top_k, 1);
                        OPT_sample_tail_free(&candidates_p, tfs_z, 1);
                        OPT_sample_typical(&candidates_p, typical_p, 1);
                        OPT_sample_top_p(&candidates_p, top_p, 1);
                        OPT_sample_temperature(&candidates_p, temp);
                        id = OPT_sample_token(&candidates_p);
                    }
                }
                // printf("`%d`", candidates_p.size);

                last_n_tokens.erase(last_n_tokens.begin());
                last_n_tokens.push_back(id);
            }

            // replace end of text token with newline token when in interactive mode
            if (id == OPT_token_eos() && params.interactive && !params.instruct) {
                // determine newline token
                auto OPT_token_newline = ::OPT_tokenize(vocab, "\n", false);
                id = OPT_token_newline.front();

                if (params.antiprompt.size() != 0) {
                    // tokenize and inject first reverse prompt
                    const auto first_antiprompt = ::OPT_tokenize(vocab, params.antiprompt.front(), false);
                    embd_inp.insert(embd_inp.end(), first_antiprompt.begin(), first_antiprompt.end());
                }
            }

            // add it to the context
            embd.push_back(id);

            // decrement remaining sampling budget
            --n_remain;
        }

        else {
            // some user input remains from prompt or interaction, forward it to processing
            while ((int)embd_inp.size() > n_consumed) {
                embd.push_back(embd_inp[n_consumed]);
                last_n_tokens.erase(last_n_tokens.begin());
                last_n_tokens.push_back(embd_inp[n_consumed]);
                ++n_consumed;

                if ((int)embd.size() >= params.n_batch) {
                    break;
                }
            }
        }
    }
}
