cmake_minimum_required(VERSION 3.4.1)

project(system_abstraction)


add_subdirectory(Application/libs/Box2D)
add_subdirectory(Application/libs/libfreetype)

add_library (system_abstraction STATIC
     system_audio.cpp
     system_abstraction.cpp
     system_paths.cpp
)

target_include_directories(system_abstraction PRIVATE
     ${CMAKE_CURRENT_SOURCE_DIR}/../
     ${ANDROID_NDK}/sources/android/native_app_glue)


target_link_libraries(system_abstraction
     Box2D
     libfreetype
     )