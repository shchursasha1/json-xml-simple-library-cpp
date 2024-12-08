cmake_minimum_required(VERSION 3.29)
project(JXSL_C C)

set(CMAKE_C_STANDARD 11)

add_executable(JXSL_C jxsl_lib.h
        jxsl_lib.c
        jxsl_lib_test.c
)
