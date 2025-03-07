cmake_minimum_required(VERSION 3.15.3)
project(test)

set(LIBS ../..)

add_executable(test test.cpp)
target_include_directories(test PRIVATE ..)
target_compile_options(test PRIVATE -std=c++17 -ggdb)
