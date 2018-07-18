cmake_minimum_required(VERSION 3.4.1)

project(system_abstraction)


include_directories(.)


# Build the Oboe library
#set (OBOE_DIR Application/libs/oboe)
#add_subdirectory (${OBOE_DIR} ./oboe)
#include_directories (${OBOE_DIR}/include)

#GLM
add_definitions("-DGLM_FORCE_SIZE_T_LENGTH -DGLM_FORCE_RADIANS")
set(GLM_STATIC_LIBRARY_ENABLE TRUE)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Application/libs)
add_subdirectory(Application/libs/glm)

include_directories(Application/libs/lib_system)
add_subdirectory(Application/libs/lib_system)

#LIB_CONFIG
include_directories(Application/libs/lib_config)
add_subdirectory(Application/libs/lib_config)

add_subdirectory(Application/libs/Box2D)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Application/libs/libfreetype/include)
add_subdirectory(Application/libs/libfreetype)

include_directories(Application/libs/SOIL/src)
add_subdirectory(Application/libs/SOIL)

include_directories(Application)
add_subdirectory(Application/CapAfri)

add_subdirectory(Application/libs/library_opengles_2)

#NUKLEAR GLES2
add_subdirectory(Application/libs/nuklear/implementation)

add_library (system_abstraction STATIC
#    oboe
    system_audio.cpp
    OpenSLWrap.cpp
    system_abstraction.cpp
    system_paths.cpp

    Application/Tunnel/anim.cpp
    Application/Tunnel/ascii_to_geom.cpp
    Application/Tunnel/dialog_scene.cpp
    Application/Tunnel/indexbuf.cpp
    Application/Tunnel/play_capafri_scene.cpp
    Application/Tunnel/map_editor_scene.cpp
    Application/Tunnel/scene.cpp
    Application/Tunnel/scene_manager.cpp
    Application/Tunnel/shader.cpp
    Application/Tunnel/shape_renderer.cpp
    Application/Tunnel/text_renderer.cpp
    Application/Tunnel/ui_scene.cpp
    Application/Tunnel/util.cpp
    Application/Tunnel/vertexbuf.cpp
    Application/Tunnel/welcome_scene.cpp
    Application/Tunnel/scene_tuning_vehicle.cpp
    )

target_include_directories(system_abstraction PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../
    ${ANDROID_NDK}/sources/android/native_app_glue)



target_link_libraries(system_abstraction
    lib_system
    CapAfri
    library_opengles_2
    Box2D
    SOIL
    libfreetype
    glm_static
    nuklear_gles2
    lib_config
    )
