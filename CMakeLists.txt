cmake_minimum_required(VERSION 3.12)
project(simpleTensor)

include_directories(../Catch2/single_include)

set(CMAKE_CXX_STANDARD 17)

if (APPLE)
    add_definitions(-DGTEST_USE_OWN_TR1_TUPLE)
    add_definitions(-D__GLIBCXX__)
endif (APPLE)

add_executable(runBasicTensorTests simple-tensor/basicTests.cpp)

add_executable(simpleTensor simple-tensor/main.cpp simple-tensor/Tensor.h)
