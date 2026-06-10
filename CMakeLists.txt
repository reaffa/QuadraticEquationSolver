cmake_minimum_required(VERSION 3.31)
project(untitled C)

set(CMAKE_C_STANDARD 11)

include_directories(include)

add_executable(untitled src/main.c
        src/performoperation.c
        src/test_suite.c)

add_executable(run_tests
        src/test_suite.c
        src/performoperation.c
        include/test_suite.h
)
