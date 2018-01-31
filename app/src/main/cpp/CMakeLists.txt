cmake_minimum_required(VERSION 3.4.1)



# build native_app_glue as a static lib
add_library(native_app_glue STATIC
     ${ANDROID_NDK}/sources/android/native_app_glue/android_native_app_glue.c)

# Export ANativeActivity_onCreate(),
# Refer to: https://github.com/android-ndk/ndk/issues/381.
set(CMAKE_SHARED_LINKER_FLAGS
    "${CMAKE_SHARED_LINKER_FLAGS} -u ANativeActivity_onCreate")


set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++11 -Wall")
add_definitions("-DGLM_FORCE_SIZE_T_LENGTH -DGLM_FORCE_RADIANS")


include_directories(${CMAKE_CURRENT_SOURCE_DIR}/SystemAbstraction/Application/libs/libfreetype/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/SystemAbstraction/Application/libs/)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/SystemAbstraction/Application/libs/SOIL/src)

add_subdirectory(SystemAbstraction/Application/libs/libfreetype)
add_subdirectory(SystemAbstraction/Application/libs/glm)
add_subdirectory(SystemAbstraction/Application/libs/SOIL)


add_library(game SHARED
     android_main.cpp
     SystemAbstraction/Application/anim.cpp
     SystemAbstraction/Application/ascii_to_geom.cpp
     SystemAbstraction/Application/dialog_scene.cpp
     SystemAbstraction/Application/indexbuf.cpp
     input_util.cpp
     jni_util.cpp
     native_engine.cpp
     SystemAbstraction/Application/obstacle.cpp
     SystemAbstraction/Application/obstacle_generator.cpp
     SystemAbstraction/Application/our_shader.cpp
     SystemAbstraction/Application/play_scene.cpp
     SystemAbstraction/Application/scene.cpp
     SystemAbstraction/Application/scene_manager.cpp
     SystemAbstraction/system_audio.cpp
     SystemAbstraction/Application/shader.cpp
     SystemAbstraction/Application/shape_renderer.cpp
     SystemAbstraction/system_abstraction.cpp
     SystemAbstraction/Application/tex_quad.cpp
     SystemAbstraction/Application/text_renderer.cpp
     SystemAbstraction/Application/texture.cpp
     SystemAbstraction/Application/ui_scene.cpp
     SystemAbstraction/Application/util.cpp
     SystemAbstraction/Application/vertexbuf.cpp
     SystemAbstraction/Application/welcome_scene.cpp)

target_include_directories(game PRIVATE
     ${CMAKE_CURRENT_SOURCE_DIR}
     ${CMAKE_CURRENT_SOURCE_DIR}/data
     ${ANDROID_NDK}/sources/android/native_app_glue)

# add lib dependencies
target_link_libraries(game
     android
     native_app_glue
     atomic
     EGL
     GLESv2
     glm
     log
     OpenSLES
     SOIL
     freetype)
