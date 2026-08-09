cmake_minimum_required(VERSION 3.0)

project(hello-cmake C)

add_executable(hello hello.c)

target_compile_options(hello PRIVATE -Wall)


