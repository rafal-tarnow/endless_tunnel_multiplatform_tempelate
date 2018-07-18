cmake_minimum_required(VERSION 3.4.1)

project(library_opengles_2)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/..)

add_library (lib_system STATIC
    fps.cpp
    )

