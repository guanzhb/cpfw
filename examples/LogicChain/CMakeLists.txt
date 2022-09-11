cmake_minimum_required(VERSION 3.5)

project(exampleChain)

set(CMAKE_CXX_STANDARD 20)

include_directories("../../cpfw/base/include"
                    "../../cpfw/core/include"
                    "../../external/tinyxml2")

FILE(GLOB BASE_SRCS "ExampleChain.cpp"
                    "../../cpfw/base/src/*"
                    "../../cpfw/core/src/*"
                    "../../external/tinyxml2/tinyxml2.cpp")

add_executable(exampleChain ${BASE_SRCS})
