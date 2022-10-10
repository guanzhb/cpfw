cmake_minimum_required(VERSION 3.5)

project(RingBufferManagerTest)

set(CMAKE_CXX_STANDARD 20)

include_directories("../../cpfw/base/include")

FILE(GLOB BASE_SRCS "RingBufferManagerTest.cpp")

link_directories("../../out")

add_executable(RingBufferManagerTest ${BASE_SRCS})

target_link_libraries(RingBufferManagerTest cpfw)
