cmake_minimum_required(VERSION 3.5)
project(edlines)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/edlines)
add_definitions(-std=c++14)

file(GLOB src "${CMAKE_CURRENT_SOURCE_DIR}/edlines/*.cpp")
add_library(edline STATIC ${src})
add_executable(edlines ${CMAKE_CURRENT_SOURCE_DIR}/test.cpp)
target_link_libraries(edlines
    edline
)