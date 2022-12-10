cmake_minimum_required(VERSION 3.5)

project(ringArrayTest)

set(CMAKE_CXX_STANDARD 20)

include_directories("../../cpfw/base/include")

FILE(GLOB BASE_SRCS "RingArrayTest.cpp")

link_directories("../../out")

add_executable(ringArrayTest ${BASE_SRCS})

target_link_libraries(ringArrayTest cpfw)
