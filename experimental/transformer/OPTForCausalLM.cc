#include "OPTForCausalLM.h"
#include "operators.h"
#include "utils.h"

float logits_output[512* 50272];
float mock_model_output_arr[512* 768];

struct OPTForCausalLM_output OPTForCausalLM::forward(const struct OPTForCausalLM_input &input){
    // Pycode: Skipped
    // output_attentions = output_attentions if output_attentions is not None else self.config.output_attentions
    // output_hidden_states = (
    //     output_hidden_states if output_hidden_states is not None else self.config.output_hidden_states
    // )
    // return_dict = return_dict if return_dict is not None else self.config.use_return_dict

    // outputs = self.model.decoder(
    //     input_ids=input_ids,
    //     attention_mask=attention_mask,
    //     head_mask=head_mask,
    //     past_key_values=past_key_values,
    //     inputs_embeds=inputs_embeds,
    //     use_cache=use_cache,
    //     output_attentions=output_attentions,
    //     output_hidden_states=output_hidden_states,
    //     return_dict=return_dict,
    // )
    // struct Int8OPTDecoder_input decoder_input = {input.input_ids};

    // struct Int8OPTDecoder_output decoder_output = this->m_decoder.forward(decoder_input);
    // TODO: Mock the output for now
    Matrix3D<float> mock_model_output(mock_model_output_arr, 1, 512, 768);
    read_to_array((char*)"sample_output.bin", mock_model_output_arr, 512 * 768);

    // logits = self.lm_head(outputs[0]).contiguous()
    Matrix3D<float> logits(logits_output, 1, 512, 50272);
    linear_fp(mock_model_output, this->lm_head_weights, logits);

    struct OPTForCausalLM_output LMoutput = {logits};
    return LMoutput;
}