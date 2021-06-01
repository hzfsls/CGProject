cmake_minimum_required(VERSION 3.17)
project(CG_Project)

set(CMAKE_CXX_STANDARD 20)

add_executable(CG_Project main.cpp workpiece.cpp workpiece.h knife.cpp knife.h knifeState.cpp knifeState.h bezierBoundary.cpp bezierBoundary.h stb_image.h particleSystem.cpp particleSystem.h)