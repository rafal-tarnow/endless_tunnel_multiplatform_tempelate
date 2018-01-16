#
# Copyright (C) The Android Open Source Project
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#

cmake_minimum_required(VERSION 3.4.1)

# build native_app_glue as a static lib
add_library(native_app_glue STATIC
     ${ANDROID_NDK}/sources/android/native_app_glue/android_native_app_glue.c)

# Export ANativeActivity_onCreate(),
# Refer to: https://github.com/android-ndk/ndk/issues/381.
set(CMAKE_SHARED_LINKER_FLAGS
    "${CMAKE_SHARED_LINKER_FLAGS} -u ANativeActivity_onCreate")

# Set common compiler options
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++11 -Wall")
add_definitions("-DGLM_FORCE_SIZE_T_LENGTH -DGLM_FORCE_RADIANS")

# Import the CMakeLists.txt for the glm library
add_subdirectory(glm)

# now build app's shared lib
add_library(game SHARED
     android_main.cpp
     SystemAbstraction/Game/anim.cpp
     SystemAbstraction/Game/ascii_to_geom.cpp
     SystemAbstraction/Game/dialog_scene.cpp
     SystemAbstraction/Game/indexbuf.cpp
     input_util.cpp
     jni_util.cpp
     native_engine.cpp
     SystemAbstraction/Game/obstacle.cpp
     SystemAbstraction/Game/obstacle_generator.cpp
     SystemAbstraction/Game/our_shader.cpp
     SystemAbstraction/Game/play_scene.cpp
     SystemAbstraction/Game/scene.cpp
     SystemAbstraction/Game/scene_manager.cpp
     SystemAbstraction/Game/sfxman.cpp
     SystemAbstraction/Game/shader.cpp
     SystemAbstraction/Game/shape_renderer.cpp
     SystemAbstraction/system_abstraction.cpp
     SystemAbstraction/Game/tex_quad.cpp
     SystemAbstraction/Game/text_renderer.cpp
     SystemAbstraction/Game/texture.cpp
     SystemAbstraction/Game/ui_scene.cpp
     SystemAbstraction/Game/util.cpp
     SystemAbstraction/Game/vertexbuf.cpp
     SystemAbstraction/Game/welcome_scene.cpp)

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
     OpenSLES)
