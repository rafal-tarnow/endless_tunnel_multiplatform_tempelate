cmake_minimum_required(VERSION 2.8)

project(lib_config)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -Wall")



add_library(${PROJECT_NAME} STATIC
	lib_config.cpp)
