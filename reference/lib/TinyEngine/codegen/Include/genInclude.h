tinyengine_status depthwise_kernel3x3_stride1_inplace_CHW_fpreq(
    q7_t *input, const uint16_t input_x, const uint16_t input_y, const uint16_t input_ch, const q7_t *kernel,
    const int32_t *bias, const int32_t *biasR, const float *scales, const int32_t output_offset,
    const int32_t input_offset, const int32_t output_activation_min, const int32_t output_activation_max, q7_t *output,
    const uint16_t output_x, const uint16_t output_y, const uint16_t output_ch, q15_t *runtime_buf, q7_t pad_value);
tinyengine_status depthwise_kernel3x3_stride1_inplace_CHW_fpreq_mask(
    q7_t *input, const uint16_t input_x, const uint16_t input_y, const uint16_t input_ch, const q7_t *kernel,
    const int32_t *bias, const int32_t *biasR, const float *scales, const int32_t output_offset,
    const int32_t input_offset, const int32_t output_activation_min, const int32_t output_activation_max, q7_t *output,
    q7_t *output_mask, const uint16_t output_x, const uint16_t output_y, const uint16_t output_ch, q15_t *runtime_buf,
    q7_t pad_value);
tinyengine_status depthwise_kernel3x3_stride1_inplace_CHW_fpreq_bitmask(
    q7_t *input, const uint16_t input_x, const uint16_t input_y, const uint16_t input_ch, const q7_t *kernel,
    const int32_t *bias, const int32_t *biasR, const float *scales, const int32_t output_offset,
    const int32_t input_offset, const int32_t output_activation_min, const int32_t output_activation_max, q7_t *output,
    q7_t *output_mask, const uint16_t output_x, const uint16_t output_y, const uint16_t output_ch, q15_t *runtime_buf,
    q7_t pad_value);
tinyengine_status depthwise_kernel3x3_stride2_inplace_CHW_fpreq(
    q7_t *input, const uint16_t input_x, const uint16_t input_y, const uint16_t input_ch, const q7_t *kernel,
    const int32_t *bias, const int32_t *biasR, const float *scales, const int32_t output_offset,
    const int32_t input_offset, const int32_t output_activation_min, const int32_t output_activation_max, q7_t *output,
    const uint16_t output_x, const uint16_t output_y, const uint16_t output_ch, q15_t *runtime_buf, q7_t pad_value);
tinyengine_status depthwise_kernel3x3_stride2_inplace_CHW_fpreq_mask(
    q7_t *input, const uint16_t input_x, const uint16_t input_y, const uint16_t input_ch, const q7_t *kernel,
    const int32_t *bias, const int32_t *biasR, const float *scales, const int32_t output_offset,
    const int32_t input_offset, const int32_t output_activation_min, const int32_t output_activation_max, q7_t *output,
    q7_t *output_mask, const uint16_t output_x, const uint16_t output_y, const uint16_t output_ch, q15_t *runtime_buf,
    q7_t pad_value);
tinyengine_status depthwise_kernel3x3_stride2_inplace_CHW_fpreq_bitmask(
    q7_t *input, const uint16_t input_x, const uint16_t input_y, const uint16_t input_ch, const q7_t *kernel,
    const int32_t *bias, const int32_t *biasR, const float *scales, const int32_t output_offset,
    const int32_t input_offset, const int32_t output_activation_min, const int32_t output_activation_max, q7_t *output,
    q7_t *output_mask, const uint16_t output_x, const uint16_t output_y, const uint16_t output_ch, q15_t *runtime_buf,
    q7_t pad_value);
tinyengine_status depthwise_kernel7x7_stride2_inplace_CHW_fpreq(
    q7_t *input, const uint16_t input_x, const uint16_t input_y, const uint16_t input_ch, const q7_t *kernel,
    const int32_t *bias, const int32_t *biasR, const float *scales, const int32_t output_offset,
    const int32_t input_offset, const int32_t output_activation_min, const int32_t output_activation_max, q7_t *output,
    const uint16_t output_x, const uint16_t output_y, const uint16_t output_ch, q15_t *runtime_buf, q7_t pad_value);
tinyengine_status depthwise_kernel7x7_stride2_inplace_CHW_fpreq_mask(
    q7_t *input, const uint16_t input_x, const uint16_t input_y, const uint16_t input_ch, const q7_t *kernel,
    const int32_t *bias, const int32_t *biasR, const float *scales, const int32_t output_offset,
    const int32_t input_offset, const int32_t output_activation_min, const int32_t output_activation_max, q7_t *output,
    q7_t *output_mask, const uint16_t output_x, const uint16_t output_y, const uint16_t output_ch, q15_t *runtime_buf,
    q7_t pad_value);
tinyengine_status depthwise_kernel7x7_stride2_inplace_CHW_fpreq_bitmask(
    q7_t *input, const uint16_t input_x, const uint16_t input_y, const uint16_t input_ch, const q7_t *kernel,
    const int32_t *bias, const int32_t *biasR, const float *scales, const int32_t output_offset,
    const int32_t input_offset, const int32_t output_activation_min, const int32_t output_activation_max, q7_t *output,
    q7_t *output_mask, const uint16_t output_x, const uint16_t output_y, const uint16_t output_ch, q15_t *runtime_buf,
    q7_t pad_value);
tinyengine_status depthwise_kernel5x5_stride1_inplace_CHW_fpreq(
    q7_t *input, const uint16_t input_x, const uint16_t input_y, const uint16_t input_ch, const q7_t *kernel,
    const int32_t *bias, const int32_t *biasR, const float *scales, const int32_t output_offset,
    const int32_t input_offset, const int32_t output_activation_min, const int32_t output_activation_max, q7_t *output,
    const uint16_t output_x, const uint16_t output_y, const uint16_t output_ch, q15_t *runtime_buf, q7_t pad_value);
tinyengine_status depthwise_kernel5x5_stride1_inplace_CHW_fpreq_mask(
    q7_t *input, const uint16_t input_x, const uint16_t input_y, const uint16_t input_ch, const q7_t *kernel,
    const int32_t *bias, const int32_t *biasR, const float *scales, const int32_t output_offset,
    const int32_t input_offset, const int32_t output_activation_min, const int32_t output_activation_max, q7_t *output,
    q7_t *output_mask, const uint16_t output_x, const uint16_t output_y, const uint16_t output_ch, q15_t *runtime_buf,
    q7_t pad_value);
tinyengine_status depthwise_kernel5x5_stride1_inplace_CHW_fpreq_bitmask(
    q7_t *input, const uint16_t input_x, const uint16_t input_y, const uint16_t input_ch, const q7_t *kernel,
    const int32_t *bias, const int32_t *biasR, const float *scales, const int32_t output_offset,
    const int32_t input_offset, const int32_t output_activation_min, const int32_t output_activation_max, q7_t *output,
    q7_t *output_mask, const uint16_t output_x, const uint16_t output_y, const uint16_t output_ch, q15_t *runtime_buf,
    q7_t pad_value);
tinyengine_status depthwise_kernel7x7_stride1_inplace_CHW_fpreq(
    q7_t *input, const uint16_t input_x, const uint16_t input_y, const uint16_t input_ch, const q7_t *kernel,
    const int32_t *bias, const int32_t *biasR, const float *scales, const int32_t output_offset,
    const int32_t input_offset, const int32_t output_activation_min, const int32_t output_activation_max, q7_t *output,
    const uint16_t output_x, const uint16_t output_y, const uint16_t output_ch, q15_t *runtime_buf, q7_t pad_value);
tinyengine_status depthwise_kernel7x7_stride1_inplace_CHW_fpreq_mask(
    q7_t *input, const uint16_t input_x, const uint16_t input_y, const uint16_t input_ch, const q7_t *kernel,
    const int32_t *bias, const int32_t *biasR, const float *scales, const int32_t output_offset,
    const int32_t input_offset, const int32_t output_activation_min, const int32_t output_activation_max, q7_t *output,
    q7_t *output_mask, const uint16_t output_x, const uint16_t output_y, const uint16_t output_ch, q15_t *runtime_buf,
    q7_t pad_value);
tinyengine_status depthwise_kernel7x7_stride1_inplace_CHW_fpreq_bitmask(
    q7_t *input, const uint16_t input_x, const uint16_t input_y, const uint16_t input_ch, const q7_t *kernel,
    const int32_t *bias, const int32_t *biasR, const float *scales, const int32_t output_offset,
    const int32_t input_offset, const int32_t output_activation_min, const int32_t output_activation_max, q7_t *output,
    q7_t *output_mask, const uint16_t output_x, const uint16_t output_y, const uint16_t output_ch, q15_t *runtime_buf,
    q7_t pad_value);
tinyengine_status depthwise_kernel5x5_stride2_inplace_CHW_fpreq(
    q7_t *input, const uint16_t input_x, const uint16_t input_y, const uint16_t input_ch, const q7_t *kernel,
    const int32_t *bias, const int32_t *biasR, const float *scales, const int32_t output_offset,
    const int32_t input_offset, const int32_t output_activation_min, const int32_t output_activation_max, q7_t *output,
    const uint16_t output_x, const uint16_t output_y, const uint16_t output_ch, q15_t *runtime_buf, q7_t pad_value);
tinyengine_status depthwise_kernel5x5_stride2_inplace_CHW_fpreq_mask(
    q7_t *input, const uint16_t input_x, const uint16_t input_y, const uint16_t input_ch, const q7_t *kernel,
    const int32_t *bias, const int32_t *biasR, const float *scales, const int32_t output_offset,
    const int32_t input_offset, const int32_t output_activation_min, const int32_t output_activation_max, q7_t *output,
    q7_t *output_mask, const uint16_t output_x, const uint16_t output_y, const uint16_t output_ch, q15_t *runtime_buf,
    q7_t pad_value);
tinyengine_status depthwise_kernel5x5_stride2_inplace_CHW_fpreq_bitmask(
    q7_t *input, const uint16_t input_x, const uint16_t input_y, const uint16_t input_ch, const q7_t *kernel,
    const int32_t *bias, const int32_t *biasR, const float *scales, const int32_t output_offset,
    const int32_t input_offset, const int32_t output_activation_min, const int32_t output_activation_max, q7_t *output,
    q7_t *output_mask, const uint16_t output_x, const uint16_t output_y, const uint16_t output_ch, q15_t *runtime_buf,
    q7_t pad_value);
