cmake_minimum_required(VERSION 3.5)

project(exampleParametersBase)

set(CMAKE_CXX_STANDARD 20)

include_directories("../../cpfw/base/include"
                    "../../cpfw/base/include/utilities"
                    "../../cpfw/core/include")

FILE(GLOB BASE_SRCS "ExampleParametersBase.cpp")

link_directories("../../out")

add_executable(exampleParametersBase ${BASE_SRCS})

target_link_libraries(exampleParametersBase cpfw)
