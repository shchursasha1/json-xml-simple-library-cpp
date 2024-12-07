cmake_minimum_required(VERSION 3.29)
project(JXSL_CPP)

set(CMAKE_CXX_STANDARD 20)

# Include paths for the C library (adjust path as needed)
include_directories("D:/Programming 2 course/Project/JXSL_C")

# Add C library source file
add_library(jxsl_lib STATIC "D:/Programming 2 course/Project/JXSL_C/jxsl_lib.c")

add_executable(JXSL_CPP
        jxsl_lib_cpp.h        # C++ header
        jxsl_lib_cpp.cpp
        tests/jxsl_lib_cpp_test.cpp# C++ implementation # C++ tests
        tests/jxsl_cross_test.cpp   # Cross-validation tests
)

# Link the C library to the C++ executable
target_link_libraries(JXSL_CPP jxsl_lib)
