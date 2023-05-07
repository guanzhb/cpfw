cmake_minimum_required(VERSION 3.5)

project(exampleThreadPoolTest)

set(CMAKE_CXX_STANDARD 20)

include_directories("../../cpfw/base/include"
                    "../../cpfw/base/include/utilities")

FILE(GLOB BASE_SRCS "ThreadPoolTest.cpp")

link_directories("../../out")

add_executable(exampleThreadPool ${BASE_SRCS})

target_link_libraries(exampleThreadPool cpfw)
