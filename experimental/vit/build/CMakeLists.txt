cmake_minimum_required(VERSION 3.10)
project(MyModel)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include_directories(${CMAKE_SOURCE_DIR})

# 소스 파일 설정
set(SOURCES
    main.cpp
    genModel.c
)

# C 소스 파일을 C++로 처리
set_source_files_properties(genModel.c PROPERTIES LANGUAGE CXX)

# 실행 파일 생성
add_executable(MyProject ${SOURCES})
