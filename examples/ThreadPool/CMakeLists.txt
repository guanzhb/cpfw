cmake_minimum_required(VERSION 3.5)

project(ThreadPoolTest)

set(CMAKE_CXX_STANDARD 20)

include_directories("../../cpfw/base/include")

FILE(GLOB BASE_SRCS "ThreadPoolTest.cpp")

link_directories("../../out")

add_executable(ThreadPoolTest ${BASE_SRCS})

target_link_libraries(ThreadPoolTest cpfw)
