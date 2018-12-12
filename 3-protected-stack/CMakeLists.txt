cmake_minimum_required(VERSION 3.9)
project(3_protected_stack)

set(CMAKE_CXX_STANDARD 11)

add_executable(3_protected_stack main.cpp Logger.h LoggerHTML.h ProtectedStack.h)