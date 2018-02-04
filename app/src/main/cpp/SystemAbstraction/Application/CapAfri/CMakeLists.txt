cmake_minimum_required(VERSION 3.4.1)

project(CapAfri)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/..)

add_library (CapAfri STATIC
    Bodies/background_body.cpp
    Bodies/car_body.cpp
    Bodies/coin_circle_body.cpp
    Bodies/coordinates_body.cpp
    Bodies/ground_body.cpp
    Bodies/ground_body_chain.cpp
    game.cpp
)