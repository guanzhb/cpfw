cmake_minimum_required(VERSION 3.5)

project(exampleChain)

set(CMAKE_CXX_STANDARD 20)

include_directories("../../cpfw/base/include"
                    "../../cpfw/core/include"
                    "../../external/tinyxml2")

FILE(GLOB BASE_SRCS "ExampleChain.cpp")

link_directories("../../out")

add_executable(exampleChain ${BASE_SRCS})

target_link_libraries(exampleChain cpfw)
