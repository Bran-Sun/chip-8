cmake_minimum_required(VERSION 3.6)
project(chip_8)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -framework OpenGL -framework glut")

set(SOURCE_FILES main.cpp chip8.cpp chip8.h)
add_executable(chip_8 ${SOURCE_FILES})