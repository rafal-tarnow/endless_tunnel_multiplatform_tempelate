cmake_minimum_required(VERSION 3.4.1)



add_library(native_app_glue STATIC
     ${ANDROID_NDK}/sources/android/native_app_glue/android_native_app_glue.c)

# Export ANativeActivity_onCreate(),
# Refer to: https://github.com/android-ndk/ndk/issues/381.
set(CMAKE_SHARED_LINKER_FLAGS
    "${CMAKE_SHARED_LINKER_FLAGS} -u ANativeActivity_onCreate")


set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++11 -Wall")

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/SystemAbstraction)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/SystemAbstraction/Application)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/SystemAbstraction/Application/libs/)


add_subdirectory(SystemAbstraction)


add_library(game SHARED
     android_main.cpp
     input_util.cpp
     jni_util.cpp
     native_engine.cpp
)

target_include_directories(game PRIVATE
     ${CMAKE_CURRENT_SOURCE_DIR}
     ${CMAKE_CURRENT_SOURCE_DIR}/data
     ${ANDROID_NDK}/sources/android/native_app_glue)


target_link_libraries(game
    system_abstraction
    android
    native_app_glue
    atomic
    EGL
    GLESv2
    log
    OpenSLES
     )
