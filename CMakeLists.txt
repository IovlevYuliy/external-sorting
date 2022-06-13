cmake_minimum_required(VERSION 3.5)

project(external-sorting LANGUAGES CXX)

set(CMAKE_CXX_FLAGS "-std=c++14 -D_NDEBUG -O3 -lpthread")
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(INCLUDES src/sorter.hpp)
set(SOURCES main.cpp src/sorter.cpp)

add_executable(sorter ${SOURCES} ${INCLUDES})