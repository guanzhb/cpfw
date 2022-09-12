cmake_minimum_required(VERSION 3.5)

project(exampleStateMachine)

set(CMAKE_CXX_STANDARD 20)

include_directories("../../cpfw/base/include")

FILE(GLOB BASE_SRCS "StateMachineExample.cpp")

link_directories("../../out")

add_executable(exampleStateMachine ${BASE_SRCS})

target_link_libraries(exampleStateMachine cpfw)
