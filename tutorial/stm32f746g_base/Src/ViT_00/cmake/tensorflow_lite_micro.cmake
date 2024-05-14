set(TENSORFLOW_LITE_DIR ${CMAKE_SOURCE_DIR}/libs/tensorflow)

# Make sure that git submodule is initialized and updated
if (NOT EXISTS "${TENSORFLOW_LITE_DIR}")
  message(FATAL_ERROR "Tensorflow-lite submodule not found. Initialize with 'git submodule update --init' in the source directory")
endif()

set (TENSORFLOW_LITE_INC
    ${TENSORFLOW_LITE_DIR}/tensorflow/lite/flatbuffers/include
    ${TENSORFLOW_LITE_DIR}/tensorflow/lite/flatbuffers/include/flatbuffers
    ${TENSORFLOW_LITE_DIR}/tensorflow/lite/gemmlowp
    ${TENSORFLOW_LITE_DIR}/tensorflow/lite/schema

    ${TENSORFLOW_LITE_DIR}
)

include_directories(
    ${TENSORFLOW_LITE_INC}
)
aux_source_directory(${TENSORFLOW_LITE_DIR}/lite/flatbuffers/src FLATBUFFERS_SRC)


if (USE_CMSIS_NN)
  set(TENSORFLOW_LITE_SRC
    ${TENSORFLOW_LITE_DIR}/tensorflow/lite/micro/kernels/cmsis-nn/add.cc
    ${TENSORFLOW_LITE_DIR}/tensorflow/lite/micro/kernels/cmsis-nn/depthwise_conv.cc
    ${TENSORFLOW_LITE_DIR}/tensorflow/lite/micro/kernels/cmsis-nn/fully_connected.cc
    ${TENSORFLOW_LITE_DIR}/tensorflow/lite/micro/kernels/cmsis-nn/mul.cc
    ${TENSORFLOW_LITE_DIR}/tensorflow/lite/micro/kernels/cmsis-nn/softmax.cc
    ${TENSORFLOW_LITE_DIR}/tensorflow/lite/micro/kernels/cmsis-nn/conv.cc
    ${TENSORFLOW_LITE_DIR}/tensorflow/lite/micro/kernels/cmsis-nn/pooling.cc
  )
else()
  set(TENSORFLOW_LITE_SRC
    ${TENSORFLOW_LITE_DIR}/tensorflow/lite/micro/kernels/add.cc
    ${TENSORFLOW_LITE_DIR}/tensorflow/lite/micro/kernels/depthwise_conv.cc
    ${TENSORFLOW_LITE_DIR}/tensorflow/lite/micro/kernels/fully_connected.cc
    ${TENSORFLOW_LITE_DIR}/tensorflow/lite/micro/kernels/mul.cc
    ${TENSORFLOW_LITE_DIR}/tensorflow/lite/micro/kernels/softmax.cc
    ${TENSORFLOW_LITE_DIR}/tensorflow/lite/micro/kernels/conv.cc
    ${TENSORFLOW_LITE_DIR}/tensorflow/lite/micro/kernels/pooling.cc
  )
endif()

# Get all source files from the Src directory
set(TENSORFLOW_LITE_SRC
    ${TENSORFLOW_LITE_SRC}
    ${TENSORFLOW_LITE_DIR}/tensorflow/lite/micro/debug_log.cc
    ${TENSORFLOW_LITE_DIR}/tensorflow/lite/micro/memory_helpers.cc
    ${TENSORFLOW_LITE_DIR}/tensorflow/lite/micro/micro_allocator.cc
    ${TENSORFLOW_LITE_DIR}/tensorflow/lite/micro/micro_error_reporter.cc
    ${TENSORFLOW_LITE_DIR}/tensorflow/lite/micro/micro_interpreter.cc
    ${TENSORFLOW_LITE_DIR}/tensorflow/lite/micro/micro_optional_debug_tools.cc
    ${TENSORFLOW_LITE_DIR}/tensorflow/lite/micro/simple_memory_allocator.cc
    ${TENSORFLOW_LITE_DIR}/tensorflow/lite/micro/test_helpers.cc
    ${TENSORFLOW_LITE_DIR}/tensorflow/lite/micro/micro_string.cc
    ${TENSORFLOW_LITE_DIR}/tensorflow/lite/micro/micro_time.cc
    ${TENSORFLOW_LITE_DIR}/tensorflow/lite/micro/micro_utils.cc

    ${TENSORFLOW_LITE_DIR}/tensorflow/lite/micro/kernels/elementwise.cc
    ${TENSORFLOW_LITE_DIR}/tensorflow/lite/micro/kernels/unpack.cc
    ${TENSORFLOW_LITE_DIR}/tensorflow/lite/micro/kernels/arg_min_max.cc
    ${TENSORFLOW_LITE_DIR}/tensorflow/lite/micro/kernels/prelu.cc
    ${TENSORFLOW_LITE_DIR}/tensorflow/lite/micro/kernels/concatenation.cc
    ${TENSORFLOW_LITE_DIR}/tensorflow/lite/micro/kernels/pack.cc
    ${TENSORFLOW_LITE_DIR}/tensorflow/lite/micro/kernels/svdf.cc
    ${TENSORFLOW_LITE_DIR}/tensorflow/lite/micro/kernels/quantize.cc
    ${TENSORFLOW_LITE_DIR}/tensorflow/lite/micro/kernels/logical.cc
    ${TENSORFLOW_LITE_DIR}/tensorflow/lite/micro/kernels/dequantize.cc
    ${TENSORFLOW_LITE_DIR}/tensorflow/lite/micro/kernels/strided_slice.cc
    ${TENSORFLOW_LITE_DIR}/tensorflow/lite/micro/kernels/comparisons.cc
    ${TENSORFLOW_LITE_DIR}/tensorflow/lite/micro/kernels/neg.cc
    ${TENSORFLOW_LITE_DIR}/tensorflow/lite/micro/kernels/ceil.cc
    ${TENSORFLOW_LITE_DIR}/tensorflow/lite/micro/kernels/reduce.cc
    ${TENSORFLOW_LITE_DIR}/tensorflow/lite/micro/kernels/activations.cc
    ${TENSORFLOW_LITE_DIR}/tensorflow/lite/micro/kernels/reshape.cc
    ${TENSORFLOW_LITE_DIR}/tensorflow/lite/micro/kernels/maximum_minimum.cc
    ${TENSORFLOW_LITE_DIR}/tensorflow/lite/micro/kernels/split.cc
    ${TENSORFLOW_LITE_DIR}/tensorflow/lite/micro/kernels/sub.cc
    ${TENSORFLOW_LITE_DIR}/tensorflow/lite/micro/kernels/round.cc
    ${TENSORFLOW_LITE_DIR}/tensorflow/lite/micro/kernels/all_ops_resolver.cc
    ${TENSORFLOW_LITE_DIR}/tensorflow/lite/micro/kernels/circular_buffer.cc
    ${TENSORFLOW_LITE_DIR}/tensorflow/lite/micro/kernels/logistic.cc
    ${TENSORFLOW_LITE_DIR}/tensorflow/lite/micro/kernels/floor.cc
    ${TENSORFLOW_LITE_DIR}/tensorflow/lite/micro/kernels/pad.cc

    ${TENSORFLOW_LITE_DIR}/tensorflow/lite/micro/memory_planner/greedy_memory_planner.cc
    ${TENSORFLOW_LITE_DIR}/tensorflow/lite/micro/memory_planner/greedy_memory_planner_test.cc
    ${TENSORFLOW_LITE_DIR}/tensorflow/lite/micro/memory_planner/linear_memory_planner_test.cc
    ${TENSORFLOW_LITE_DIR}/tensorflow/lite/micro/memory_planner/linear_memory_planner.cc
    
    ${TENSORFLOW_LITE_DIR}/tensorflow/lite/c/common.c

    ${TENSORFLOW_LITE_DIR}/tensorflow/lite/core/api/error_reporter.cc
    ${TENSORFLOW_LITE_DIR}/tensorflow/lite/core/api/flatbuffer_conversions.cc
    ${TENSORFLOW_LITE_DIR}/tensorflow/lite/core/api/op_resolver.cc
    ${TENSORFLOW_LITE_DIR}/tensorflow/lite/core/api/tensor_utils.cc
    
    ${TENSORFLOW_LITE_DIR}/tensorflow/lite/kernels/internal/quantization_util.cc
    ${TENSORFLOW_LITE_DIR}/tensorflow/lite/kernels/kernel_util.cc
    ${TENSORFLOW_LITE_DIR}/tensorflow/lite/micro/testing/test_utils.cc

    ${FLATBUFFERS_SRC}
)

# set_source_files_properties(${TENSORFLOW_LITE_SRC}
#     PROPERTIES COMPILE_FLAGS "${STM32F7_DEFINES} -Wno-fpermissive"
# )



add_library(Tensorflow_lite_micro STATIC ${TENSORFLOW_LITE_SRC})
target_link_libraries(Tensorflow_lite_micro
  ${EXTERNAL_LIBS}
)

set_target_properties(Tensorflow_lite_micro PROPERTIES LINKER_LANGUAGE CXX)

set(EXTERNAL_EXECUTABLES ${EXTERNAL_EXECUTABLES} ${STARTUP_ASM_FILE})

set(EXTERNAL_LIBS ${EXTERNAL_LIBS} Tensorflow_lite_micro)
