cmake_minimum_required(VERSION 2.8)
project(lexer-cpsl)
set (CMAKE_CXX_FLAGS "-lfl")
add_executable(lexer-cpsl lex.yy.cpp)
