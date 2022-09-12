cmake_minimum_required(VERSION 3.5)

project(cpfw)

set(CMAKE_CXX_STANDARD 20)

include_directories("base/include"
                    "core/include"
                    "../external/tinyxml2")

FILE(GLOB BASE_SRCS "base/src/*"
                    "core/src/*"
                    "../external/tinyxml2/tinyxml2.cpp")

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ../out/)

add_library(cpfw SHARED ${BASE_SRCS})
