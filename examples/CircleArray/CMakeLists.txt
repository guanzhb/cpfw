cmake_minimum_required(VERSION 3.5)

project(CircleArrayTest)

set(CMAKE_CXX_STANDARD 20)

include_directories("../../cpfw/base/include")

FILE(GLOB BASE_SRCS "CircleArrayTest.cpp")

add_executable(CircleArrayTest ${BASE_SRCS})
