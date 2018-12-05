cmake_minimum_required(VERSION 3.9)
project(5_differentiator)

set(CMAKE_CXX_STANDARD 14)

add_executable(5_differentiator main.cpp Tree.h Parser.h Differentiator.h Simplificator.h Validator.h TexLogger.h GlobalVar.h)