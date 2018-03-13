cmake_minimum_required(VERSION 2.8)

project(lib_config)

if(CMAKE_COMPILER_IS_GNUCXX)
    add_definitions(-std=gnu++0x)
endif()

add_library(${PROJECT_NAME} STATIC
	lib_config.cpp)
