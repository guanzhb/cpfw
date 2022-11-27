cmake_minimum_required(VERSION 3.5)

project(SingletonTest)

set(CMAKE_CXX_STANDARD 20)

include_directories("../../cpfw/base/include")

FILE(GLOB BASE_SRCS "SingletonTest.cpp")

link_directories("../../out")

add_executable(SingletonTest ${BASE_SRCS})

target_link_libraries(SingletonTest cpfw)
