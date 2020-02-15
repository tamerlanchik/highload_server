cmake_minimum_required(VERSION 3.10)
project(HighLoad)

set(CMAKE_CXX_STANDARD 17)
include_directories(./include)

add_executable(HighLoad src/main.cpp src/ConfigAdapter.cpp src/ConfigParser.cpp)