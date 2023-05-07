cmake_minimum_required(VERSION 3.5)

project(exampleSingleton)

set(CMAKE_CXX_STANDARD 20)

include_directories("../../cpfw/base/include"
                    "../../cpfw/base/include/utilities")

FILE(GLOB BASE_SRCS "SingletonTest.cpp")

link_directories("../../out")

add_executable(exampleSingleton ${BASE_SRCS})

target_link_libraries(exampleSingleton cpfw)
