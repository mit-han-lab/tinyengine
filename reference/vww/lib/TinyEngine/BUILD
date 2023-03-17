cc_library(
    name = "tinyengine",
    srcs = ["src/add_fpreq.c",
            "src/convolve_1x1_s8_fpreq.c",
            "src/convolve_s8_kernel3_inputch3_stride2_pad1_fpreq.c",
            "src/mat_mul_kernels_fpreq.c",
            "src/avgpooling.c",
            "src/common.c",
            "codegen/Source/depthwise_kernel3x3_stride1_inplace_CHW_fpreq.c",
            "codegen/Source/depthwise_kernel3x3_stride2_inplace_CHW_fpreq.c",
            "codegen/Source/depthwise_kernel5x5_stride1_inplace_CHW_fpreq.c",
            "codegen/Source/depthwise_kernel7x7_stride1_inplace_CHW_fpreq.c",
            "codegen/Source/depthwise_kernel7x7_stride2_inplace_CHW_fpreq.c",
            "codegen/Source/genModel.c",
            ],
    hdrs = ["include/tinyengine_function.h",
            "include/tinyengine_function_fp.h",
            "include/genNN.h",
            "include/fp_requantize_op.h",
            "include/yoloOutput.h",
            "codegen/Include/genModel.h",
            "codegen/Include/genInclude.h",],
    copts = ["-Ilib/TinyEngine/include", "-Ilib/TinyEngine/codegen/Include"],
    visibility = ["//application:__pkg__"],
)

cc_library(
    name = "reference",
    srcs = ["src/reference.cc"],
    hdrs = ["include/tinyengine_function.h",
            "include/reference.h",
            "include/fp_requantize_op.h",
            "codegen/Include/genInclude.h",],
    copts = ["-Ilib/TinyEngine/include"],
)

cc_test(
    name = "tinyengine_kernel_test",
    srcs = ["test/src/convolve_1x1_s8_fpreq_test.cc",
            "test/src/convolve_s8_kernel3_inputch3_stride2_pad1_fpreq_test.cc"],
    copts = ["-Ilib/TinyEngine/include"],
    deps = ["@com_google_googletest//:gtest_main", ":reference", ":tinyengine"],
)
