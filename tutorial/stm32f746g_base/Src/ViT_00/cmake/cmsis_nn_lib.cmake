set(NN_LIB_DIR ${CMAKE_SOURCE_DIR}/libs/CMSIS/NN)
set(DSP_LIB_DIR ${CMAKE_SOURCE_DIR}/libs/CMSIS/DSP)

# Make sure the lib is there
if (NOT EXISTS "${NN_LIB_DIR}")
  message(FATAL_ERROR "NN_LIB_DIR not found.")
endif()

include_directories(
    ${NN_LIB_DIR}/Include
    ${DSP_LIB_DIR}/Include
    ${DSP_LIB_DIR}/Core/Include
)

# Get all source files from the Source directory
set(NN_LIB_SRC
    ${NN_LIB_DIR}/Source/ReshapeFunctions/arm_reshape_s8.c
    
    ${NN_LIB_DIR}/Source/ConvolutionFunctions/arm_convolve_HWC_q7_fast.c
    ${NN_LIB_DIR}/Source/ConvolutionFunctions/arm_nn_depthwise_conv_s8_core.c
    ${NN_LIB_DIR}/Source/ConvolutionFunctions/arm_convolve_HWC_q15_fast.c
    ${NN_LIB_DIR}/Source/ConvolutionFunctions/arm_depthwise_conv_s8.c
    ${NN_LIB_DIR}/Source/ConvolutionFunctions/arm_nn_mat_mult_kernel_q7_q15_reordered.c
    ${NN_LIB_DIR}/Source/ConvolutionFunctions/arm_convolve_s8.c
    ${NN_LIB_DIR}/Source/ConvolutionFunctions/arm_depthwise_separable_conv_HWC_q7_nonsquare.c
    ${NN_LIB_DIR}/Source/ConvolutionFunctions/arm_nn_mat_mult_kernel_q7_q15.c
    ${NN_LIB_DIR}/Source/ConvolutionFunctions/arm_convolve_HWC_q7_RGB.c
    ${NN_LIB_DIR}/Source/ConvolutionFunctions/arm_convolve_1x1_s8_fast.c
    ${NN_LIB_DIR}/Source/ConvolutionFunctions/arm_convolve_1_x_n_s8.c
    ${NN_LIB_DIR}/Source/ConvolutionFunctions/arm_convolve_HWC_q7_fast_nonsquare.c
    
    ${NN_LIB_DIR}/Source/ConvolutionFunctions/arm_nn_mat_mult_s8.c
    ${NN_LIB_DIR}/Source/ConvolutionFunctions/arm_convolve_1x1_HWC_q7_fast_nonsquare.c
    ${NN_LIB_DIR}/Source/ConvolutionFunctions/arm_nn_mat_mult_kernel_s8_s16_reordered.c
    ${NN_LIB_DIR}/Source/ConvolutionFunctions/arm_convolve_HWC_q7_basic_nonsquare.c
    ${NN_LIB_DIR}/Source/ConvolutionFunctions/arm_depthwise_conv_u8_basic_ver1.c
    ${NN_LIB_DIR}/Source/ConvolutionFunctions/arm_depthwise_conv_s8_opt.c
    ${NN_LIB_DIR}/Source/ConvolutionFunctions/arm_depthwise_conv_3x3_s8.c
    ${NN_LIB_DIR}/Source/ConvolutionFunctions/arm_convolve_HWC_q15_basic.c
    ${NN_LIB_DIR}/Source/ConvolutionFunctions/arm_convolve_HWC_q7_basic.c
    ${NN_LIB_DIR}/Source/ConvolutionFunctions/arm_convolve_HWC_q15_fast_nonsquare.c
    ${NN_LIB_DIR}/Source/ConvolutionFunctions/arm_nn_mat_mult_kernel_s8_s16.c
    ${NN_LIB_DIR}/Source/ConvolutionFunctions/arm_depthwise_separable_conv_HWC_q7.c
    
    ${NN_LIB_DIR}/Source/FullyConnectedFunctions/arm_fully_connected_q15.c
    ${NN_LIB_DIR}/Source/FullyConnectedFunctions/arm_fully_connected_mat_q7_vec_q15_opt.c
    ${NN_LIB_DIR}/Source/FullyConnectedFunctions/arm_fully_connected_q7_opt.c
    ${NN_LIB_DIR}/Source/FullyConnectedFunctions/arm_fully_connected_q7.c
    ${NN_LIB_DIR}/Source/FullyConnectedFunctions/arm_fully_connected_s8.c
    ${NN_LIB_DIR}/Source/FullyConnectedFunctions/arm_fully_connected_mat_q7_vec_q15.c
    ${NN_LIB_DIR}/Source/FullyConnectedFunctions/arm_fully_connected_q15_opt.c
    
    ${NN_LIB_DIR}/Source/ConcatenationFunctions/arm_concatenation_s8_y.c
    ${NN_LIB_DIR}/Source/ConcatenationFunctions/arm_concatenation_s8_w.c
    ${NN_LIB_DIR}/Source/ConcatenationFunctions/arm_concatenation_s8_z.c
    ${NN_LIB_DIR}/Source/ConcatenationFunctions/arm_concatenation_s8_x.c
    
    ${NN_LIB_DIR}/Source/NNSupportFunctions/arm_nn_vec_mat_mult_t_s8.c
    ${NN_LIB_DIR}/Source/NNSupportFunctions/arm_nn_depthwise_conv_nt_t_s8.c
    ${NN_LIB_DIR}/Source/NNSupportFunctions/arm_nn_depthwise_conv_nt_t_padded_s8.c
    ${NN_LIB_DIR}/Source/NNSupportFunctions/arm_nn_accumulate_q7_to_q15.c
    ${NN_LIB_DIR}/Source/NNSupportFunctions/arm_nntables.c
    ${NN_LIB_DIR}/Source/NNSupportFunctions/arm_nn_mat_mul_core_1x_s8.c
    ${NN_LIB_DIR}/Source/NNSupportFunctions/arm_nn_mat_mul_core_4x_s8.c
    ${NN_LIB_DIR}/Source/NNSupportFunctions/arm_q7_to_q15_no_shift.c
    ${NN_LIB_DIR}/Source/NNSupportFunctions/arm_nn_mult_q15.c
    ${NN_LIB_DIR}/Source/NNSupportFunctions/arm_q7_to_q15_with_offset.c
    ${NN_LIB_DIR}/Source/NNSupportFunctions/arm_q7_to_q15_reordered_no_shift.c
    ${NN_LIB_DIR}/Source/NNSupportFunctions/arm_q7_to_q15_reordered_with_offset.c
    ${NN_LIB_DIR}/Source/NNSupportFunctions/arm_nn_mult_q7.c
    ${NN_LIB_DIR}/Source/NNSupportFunctions/arm_nn_add_q7.c
    ${NN_LIB_DIR}/Source/NNSupportFunctions/arm_nn_mat_mult_nt_t_s8.c
    
    ${NN_LIB_DIR}/Source/BasicMathFunctions/arm_elementwise_mul_s8.c
    ${NN_LIB_DIR}/Source/BasicMathFunctions/arm_elementwise_add_s8.c
    
    ${NN_LIB_DIR}/Source/ActivationFunctions/arm_relu_q15.c
    ${NN_LIB_DIR}/Source/ActivationFunctions/arm_relu6_s8.c
    ${NN_LIB_DIR}/Source/ActivationFunctions/arm_relu_q7.c
    ${NN_LIB_DIR}/Source/ActivationFunctions/arm_nn_activations_q7.c
    ${NN_LIB_DIR}/Source/ActivationFunctions/arm_nn_activations_q15.c
    
    ${NN_LIB_DIR}/Source/SoftmaxFunctions/arm_softmax_s8.c
    ${NN_LIB_DIR}/Source/SoftmaxFunctions/arm_softmax_u8.c
    ${NN_LIB_DIR}/Source/SoftmaxFunctions/arm_softmax_q15.c
    ${NN_LIB_DIR}/Source/SoftmaxFunctions/arm_softmax_with_batch_q7.c
    ${NN_LIB_DIR}/Source/SoftmaxFunctions/arm_softmax_q7.c
    
    ${NN_LIB_DIR}/Source/PoolingFunctions/arm_max_pool_s8_opt.c
    ${NN_LIB_DIR}/Source/PoolingFunctions/arm_max_pool_s8.c
    ${NN_LIB_DIR}/Source/PoolingFunctions/arm_pool_q7_HWC.c
    ${NN_LIB_DIR}/Source/PoolingFunctions/arm_avgpool_s8.c
)

set_source_files_properties(${NN_LIB_SRC}
    PROPERTIES COMPILE_FLAGS ${STM32F7_DEFINES}
)

add_library(cmsis_nn_lib STATIC ${NN_LIB_SRC})
set_target_properties(cmsis_nn_lib PROPERTIES LINKER_LANGUAGE C)

set(EXTERNAL_LIBS ${EXTERNAL_LIBS} cmsis_nn_lib)