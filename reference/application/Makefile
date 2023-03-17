CXX = g++
CXXFLAGS = -std=c++14
CFLAGS = -Wall -ansi -O3
OPTFLAG =
INCLUDE = ../lib/TinyEngine/include
SOURCE = ../lib/TinyEngine/src
CODEGEN_INCLUDE = ../lib/TinyEngine/codegen/Include
CODEGEN_SOURCE = ../lib/TinyEngine/codegen/Source
# Please set up your own path for OpenCV
OPENCV_PATH = /usr/local/Cellar/opencv/4.7.0_1
OPENCV_INCLUDE = $(OPENCV_PATH)/include/opencv4/
OPENCV_LIB = -L$(OPENCV_PATH)/lib
OPENCV_LINK = -lopencv_gapi -lopencv_stitching -lopencv_alphamat -lopencv_aruco -lopencv_barcode -lopencv_bgsegm -lopencv_bioinspired \
			-lopencv_ccalib -lopencv_dnn_objdetect -lopencv_dnn_superres -lopencv_dpm -lopencv_face -lopencv_freetype -lopencv_fuzzy \
			-lopencv_hfs -lopencv_img_hash -lopencv_intensity_transform -lopencv_line_descriptor -lopencv_mcc -lopencv_quality \
			-lopencv_rapid -lopencv_reg -lopencv_rgbd -lopencv_saliency -lopencv_sfm -lopencv_stereo -lopencv_structured_light \
			-lopencv_phase_unwrapping -lopencv_superres -lopencv_optflow -lopencv_surface_matching -lopencv_tracking -lopencv_highgui \
			-lopencv_datasets -lopencv_text -lopencv_plot -lopencv_videostab -lopencv_videoio -lopencv_viz -lopencv_wechat_qrcode \
			-lopencv_xfeatures2d -lopencv_shape -lopencv_ml -lopencv_ximgproc -lopencv_video -lopencv_xobjdetect -lopencv_objdetect \
			-lopencv_calib3d -lopencv_imgcodecs -lopencv_features2d -lopencv_dnn -lopencv_flann -lopencv_xphoto -lopencv_photo \
			-lopencv_imgproc -lopencv_core  # Here we list most of the linkers for convenience. Feel free to trim unnecessary ones, if needed.
LIB = -L../lib/TinyEngine/codegen/Source -L../lib/TinyEngine/src
OBJECTS = main.o add_fpreq.o convolve_1x1_s8_fpreq.o convolve_s8_kernel3_inputch3_stride2_pad1_fpreq.o mat_mul_kernels_fpreq.o avgpooling.o \
		depthwise_kernel3x3_stride1_inplace_CHW_fpreq.o depthwise_kernel3x3_stride2_inplace_CHW_fpreq.o \
		depthwise_kernel5x5_stride1_inplace_CHW_fpreq.o \
		depthwise_kernel7x7_stride1_inplace_CHW_fpreq.o depthwise_kernel7x7_stride2_inplace_CHW_fpreq.o \
		genModel.o reference.o common.o

.SUFFIXS: .c .cc .cpp
main: clean $(OBJECTS)
	@$(CXX) $(CXXFLAGS) $(CFLAGS) $(OBJECTS) -o main -I$(INCLUDE) -I$(CODEGEN_INCLUDE) $(LIB) -I$(OPENCV_INCLUDE) $(OPENCV_LIB) $(OPENCV_LINK)
	@echo "Compilation and build completed - [MIT Hanlab] Platform-independent TinyEngine Demo"

main.o: main.cc
	@$(CXX) $(CXXFLAGS) -Wall -Ofast -c main.cc -I$(INCLUDE) -I$(CODEGEN_INCLUDE) -I$(OPENCV_INCLUDE)

common.o: $(SOURCE)/common.c
	@$(CXX) $(CXXFLAGS) $(CFLAGS) -c $(SOURCE)/common.c -I$(INCLUDE)

add_fpreq.o: $(SOURCE)/add_fpreq.c
	@$(CXX) $(CXXFLAGS) $(CFLAGS) -c $(SOURCE)/add_fpreq.c -I$(INCLUDE)

convolve_1x1_s8_fpreq.o: $(SOURCE)/convolve_1x1_s8_fpreq.c
	@$(CXX) $(CXXFLAGS) $(CFLAGS) -c $(SOURCE)/convolve_1x1_s8_fpreq.c -I$(INCLUDE) -D $(OPTFLAG)

convolve_s8_kernel3_inputch3_stride2_pad1_fpreq.o: $(SOURCE)/convolve_s8_kernel3_inputch3_stride2_pad1_fpreq.c
	@$(CXX) $(CXXFLAGS) $(CFLAGS) -c $(SOURCE)/convolve_s8_kernel3_inputch3_stride2_pad1_fpreq.c -I$(INCLUDE)

mat_mul_kernels_fpreq.o: $(SOURCE)/mat_mul_kernels_fpreq.c
	@$(CXX) $(CXXFLAGS) $(CFLAGS) -c $(SOURCE)/mat_mul_kernels_fpreq.c -I$(INCLUDE)

avgpooling.o: $(SOURCE)/avgpooling.c
	@$(CXX) $(CXXFLAGS) $(CFLAGS) -c $(SOURCE)/avgpooling.c -I$(INCLUDE)

reference.o: $(SOURCE)/reference.cc
	@$(CXX) $(CXXFLAGS) $(CFLAGS) -c $(SOURCE)/reference.cc -I$(INCLUDE)

depthwise_kernel3x3_stride1_inplace_CHW_fpreq.o: $(CODEGEN_SOURCE)/depthwise_kernel3x3_stride1_inplace_CHW_fpreq.c
	@$(CXX) $(CXXFLAGS) $(CFLAGS) -c $(CODEGEN_SOURCE)/depthwise_kernel3x3_stride1_inplace_CHW_fpreq.c -I$(INCLUDE)

depthwise_kernel3x3_stride2_inplace_CHW_fpreq.o: $(CODEGEN_SOURCE)/depthwise_kernel3x3_stride2_inplace_CHW_fpreq.c
	@$(CXX) $(CXXFLAGS) $(CFLAGS) -c $(CODEGEN_SOURCE)/depthwise_kernel3x3_stride2_inplace_CHW_fpreq.c -I$(INCLUDE)

depthwise_kernel5x5_stride1_inplace_CHW_fpreq.o: $(CODEGEN_SOURCE)/depthwise_kernel5x5_stride1_inplace_CHW_fpreq.c
	@$(CXX) $(CXXFLAGS) $(CFLAGS) -c $(CODEGEN_SOURCE)/depthwise_kernel5x5_stride1_inplace_CHW_fpreq.c -I$(INCLUDE)

# depthwise_kernel5x5_stride2_inplace_CHW_fpreq.o: $(CODEGEN_SOURCE)/depthwise_kernel5x5_stride2_inplace_CHW_fpreq.c
# 	@$(CXX) $(CXXFLAGS) $(CFLAGS) -c $(CODEGEN_SOURCE)/depthwise_kernel5x5_stride2_inplace_CHW_fpreq.c -I$(INCLUDE)

depthwise_kernel7x7_stride1_inplace_CHW_fpreq.o: $(CODEGEN_SOURCE)/depthwise_kernel7x7_stride1_inplace_CHW_fpreq.c
	@$(CXX) $(CXXFLAGS) $(CFLAGS) -c $(CODEGEN_SOURCE)/depthwise_kernel7x7_stride1_inplace_CHW_fpreq.c -I$(INCLUDE)

depthwise_kernel7x7_stride2_inplace_CHW_fpreq.o: $(CODEGEN_SOURCE)/depthwise_kernel7x7_stride2_inplace_CHW_fpreq.c
	@$(CXX) $(CXXFLAGS) $(CFLAGS) -c $(CODEGEN_SOURCE)/depthwise_kernel7x7_stride2_inplace_CHW_fpreq.c -I$(INCLUDE)

genModel.o: $(CODEGEN_SOURCE)/genModel.c
	@$(CXX) $(CXXFLAGS) $(CFLAGS) -c $(CODEGEN_SOURCE)/genModel.c -I$(INCLUDE) -I$(CODEGEN_INCLUDE)

.PHONY: main clean
clean:
	-rm *.o main
