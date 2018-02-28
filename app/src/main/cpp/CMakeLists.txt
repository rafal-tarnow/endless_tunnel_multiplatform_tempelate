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

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/SystemAbstraction)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/SystemAbstraction/Application)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/SystemAbstraction/Application/libs/libfreetype/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/SystemAbstraction/Application/libs/)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/SystemAbstraction/Application/libs/SOIL/src)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/SystemAbstraction/Application/libs)

add_subdirectory(SystemAbstraction)
add_subdirectory(SystemAbstraction/Application/CapAfri)
add_subdirectory(SystemAbstraction/Application/libs/libfreetype)
add_subdirectory(SystemAbstraction/Application/libs/glm)
add_subdirectory(SystemAbstraction/Application/libs/SOIL)
add_subdirectory(SystemAbstraction/Application/libs/library_opengles_2)
add_subdirectory(SystemAbstraction/Application/libs/Box2D)

add_library(game SHARED
     android_main.cpp
     SystemAbstraction/Application/Tunnel/anim.cpp
     SystemAbstraction/Application/Tunnel/ascii_to_geom.cpp
     SystemAbstraction/Application/Tunnel/dialog_scene.cpp
     SystemAbstraction/Application/Tunnel/indexbuf.cpp
     input_util.cpp
     jni_util.cpp
     native_engine.cpp
     SystemAbstraction/Application/Tunnel/obstacle.cpp
     SystemAbstraction/Application/Tunnel/obstacle_generator.cpp
     SystemAbstraction/Application/Tunnel/our_shader.cpp
     SystemAbstraction/Application/Tunnel/play_scene.cpp
     SystemAbstraction/Application/Tunnel/play_capafri_scene.cpp
     SystemAbstraction/Application/Tunnel/map_editor_scene.cpp
     SystemAbstraction/Application/Tunnel/scene.cpp
     SystemAbstraction/Application/Tunnel/scene_manager.cpp
     SystemAbstraction/Application/Tunnel/shader.cpp
     SystemAbstraction/Application/Tunnel/shape_renderer.cpp
     SystemAbstraction/Application/Tunnel/tex_quad.cpp
     SystemAbstraction/Application/Tunnel/text_renderer.cpp
     SystemAbstraction/Application/Tunnel/texture.cpp
     SystemAbstraction/Application/Tunnel/ui_scene.cpp
     SystemAbstraction/Application/Tunnel/util.cpp
     SystemAbstraction/Application/Tunnel/vertexbuf.cpp
     SystemAbstraction/Application/Tunnel/welcome_scene.cpp)

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
    # glm
     log
     OpenSLES
     CapAfri
     SOIL
     library_opengles_2
     libfreetype
     Box2D
     )
