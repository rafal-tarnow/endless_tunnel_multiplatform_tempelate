cmake_minimum_required (VERSION 2.6)

project (linux_endless_tunnel)


if(ANDROID)

else()
    add_custom_target(copy-runtime-files ALL
        COMMAND cmake -E copy_directory ${CMAKE_SOURCE_DIR}/../RESOURCES/assets ${CMAKE_BINARY_DIR}/assets)
endif()



set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++11 -Wall")

include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../app/src/main/cpp/SystemAbstraction")

add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/../app/src/main/cpp/SystemAbstraction" "${CMAKE_CURRENT_BINARY_DIR}/app/src/main/cpp/SystemAbstraction")

add_executable(linux_endless_tunnel main.cpp)

target_link_libraries(linux_endless_tunnel
    system_abstraction
    GLESv2
    glfw
    )
