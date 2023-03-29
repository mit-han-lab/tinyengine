CXX = g++
CXXFLAGS = -std=c++14
CFLAGS = -Wall -ansi
INCLUDE = ../include
SOURCE = ../src
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
LIB = -L../src
BATCH_OBJECTS = main_batch.o batch_model.o
OBJECTS = main.o model.o

.SUFFIXS: .c .cc .cpp
main: clean $(OBJECTS) $(BATCH_OBJECTS)
	@$(CXX) $(CXXFLAGS) $(CFLAGS) $(OBJECTS) -o main -I$(INCLUDE) $(LIB) -I$(OPENCV_INCLUDE) $(OPENCV_LIB) $(OPENCV_LINK)
	@$(CXX) $(CXXFLAGS) $(CFLAGS) $(BATCH_OBJECTS) -o main_batch -I$(INCLUDE) $(LIB) -I$(OPENCV_INCLUDE) $(OPENCV_LIB) $(OPENCV_LINK)
	@echo "Compilation and build completed - [MIT Hanlab] Platform-independent TinyEngine MNIST Demo"

main.o: main.cc
	@$(CXX) $(CXXFLAGS) -Wall -Ofast -c main.cc -I$(INCLUDE) -I$(OPENCV_INCLUDE)

model.o: $(SOURCE)/model.cc
	@$(CXX) $(CXXFLAGS) $(CFLAGS) -c $(SOURCE)/model.cc -I$(INCLUDE)

main_batch.o: main_batch.cc
	@$(CXX) $(CXXFLAGS) -Wall -Ofast -c main_batch.cc -I$(INCLUDE) -I$(OPENCV_INCLUDE)

batch_model.o: $(SOURCE)/batch_model.cc
	@$(CXX) $(CXXFLAGS) $(CFLAGS) -c $(SOURCE)/batch_model.cc -I$(INCLUDE)

.PHONY: main clean
clean:
	-rm *.o main main_batch
