cmake_minimum_required(VERSION 3.5)
project(thread)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES source.cpp)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "/home/vimikeva/ClionProjects/thread")
add_executable(thread ${SOURCE_FILES})
target_link_libraries(thread pthread)
