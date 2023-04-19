This folder contains the experimental features of transformer support. Right now the network architecture is manually implemented, and we will switch to code generation with smoothquant support.

We use opt-125m as the example to implement the cpp counterparts of the following features (only necessary parts for inference):
* OPTForCausalLM class in fransformers
    * OPTForCausalLM is in transfomers/models/opt/modeling_opt.py
* Int8OPTDecoder class in smoothquant
    * Int8OPTDecoder is in smoothquant/opt.py
    * Handling the cases where input_len % 16 !=0
    * It basically a warpper arounds the old_forward function in OPTDecoder.forward
* OPTDecoder class in fransformers
    * OPTDecoder is in transfomers/models/opt/modeling_opt.py
    * The OPTDecoder runs each decoder layer and return the last hidden state, key_values, all hidden states and all self-attentions.
* Int8OPTAttention class in smoothquant
    * OPTDecoder is in transfomers/models/opt/modeling_opt.py
    * 6 major componets
        * qk_bmm: batch int8 matmul with fp32 output
        * pv_bmm: batch int8 matmul with int8 output
        * k_proj, v_proj, out_proj: W8A8B8O8Linear(embed_dim, enbed_dim)

Note: the calling stacks to run opt-125m in the smoothquant example, only show the main inputs here.
Evaluator.evaluate ->(input_ids) OPTForCausalLM.forward ->(input_ids) Int8OPTDecoder.forward ->(input_ids) OPTDecoder.forward ->(hidden_states) Int8OPTDecoderLayer ->(hidden_states) Int8OPTAttention.forward 

* hidden_states = inputs_embeds + pos_embeds, (inputs_embeds = self.embed_tokens(input_ids))

