#include "Int8OPTDecoder.h"
#include "OPTDecoder.h"


Int8OPTDecoder::Int8OPTDecoder(){

}

struct Int8OPTDecoder_output Int8OPTDecoder::forward(const struct Int8OPTDecoder_input &input){
    // TODO P2: handle input_len%16 != 0
    // pad the input to the multiple of 16

    // OPTDecoder:
    

    // TODO P2: slice the output to the original length
    struct Int8OPTDecoder_output output;
    return output;
}
