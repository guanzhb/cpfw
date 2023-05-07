cmake_minimum_required(VERSION 3.5)

project(exampleSimpleLogic)

set(CMAKE_CXX_STANDARD 20)

include_directories("../../cpfw/base/include"
                    "../../cpfw/base/include/utilities"
                    "../../cpfw/core/include"
                    "../../external/tinyxml2")

FILE(GLOB BASE_SRCS "SimpleLogicExample.cpp")

link_directories("../../out")

add_executable(exampleSimpleLogic ${BASE_SRCS})

target_link_libraries(exampleSimpleLogic cpfw)
