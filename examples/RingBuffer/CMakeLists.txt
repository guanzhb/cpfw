cmake_minimum_required(VERSION 3.5)

project(RingBufferTest)

set(CMAKE_CXX_STANDARD 20)

include_directories("../../cpfw/base/include")

FILE(GLOB BASE_SRCS "RingBufferTest.cpp")

add_executable(RingBufferTest ${BASE_SRCS})
