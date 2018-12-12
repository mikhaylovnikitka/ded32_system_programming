cmake_minimum_required(VERSION 3.9)
project(4_processor)

set(CMAKE_CXX_STANDARD 14)

add_executable(4_processor main.cpp Commands/Commands.h DefineMagic/StrSwitch.h Commands/GlobalVariables.h CodeToByte/CodeToByte.h TextClass/TextClass.h Commands/Jump.h)