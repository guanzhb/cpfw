cmake_minimum_required(VERSION 3.5)

project(RingArrayTest)

set(CMAKE_CXX_STANDARD 20)

include_directories("../../cpfw/base/include")

FILE(GLOB BASE_SRCS "RingArrayTest.cpp")

add_executable(RingArrayTest ${BASE_SRCS})
