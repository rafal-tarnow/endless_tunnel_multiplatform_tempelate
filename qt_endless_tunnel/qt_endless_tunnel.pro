DEFINES += GLM_FORCE_SIZE_T_LENGTH
DEFINES += GLM_FORCE_RADIANS

include(../app/src/main/cpp/SystemAbstraction/Application/libs/library_opengles_2/library_opengles_2.pri)
include(../app/src/main/cpp/SystemAbstraction/Application/libs/libfreetype/libfreetype.pri)
include(../app/src/main/cpp/SystemAbstraction/Application/libs/SOIL/soil_qt.pri)
include(../app/src/main/cpp/SystemAbstraction/Application/libs/glm/glm_qt.pri)
include(../app/src/main/cpp/SystemAbstraction/Application/libs/Box2D/Box2D.pri)
include(../app/src/main/cpp/SystemAbstraction/Application/CapAfri/CapAfri.pri)

TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

LIBS += -lX11
LIBS += -lXrender
LIBS += -lGLEW
LIBS += -lGL

LIBS += -lGLESv2
LIBS += -lglfw


INCLUDEPATH += . ../app/src/main/cpp/SystemAbstraction/Application/


SOURCES += main.cpp \
    ../app/src/main/cpp/SystemAbstraction/Application/Tunnel/anim.cpp \
    ../app/src/main/cpp/SystemAbstraction/Application/Tunnel/ascii_to_geom.cpp \
    ../app/src/main/cpp/SystemAbstraction/Application/Tunnel/dialog_scene.cpp \
    ../app/src/main/cpp/SystemAbstraction/Application/Tunnel/indexbuf.cpp \
    ../app/src/main/cpp/SystemAbstraction/Application/Tunnel/obstacle.cpp \
    ../app/src/main/cpp/SystemAbstraction/Application/Tunnel/obstacle_generator.cpp \
    ../app/src/main/cpp/SystemAbstraction/Application/Tunnel/our_shader.cpp \
    ../app/src/main/cpp/SystemAbstraction/Application/Tunnel/play_scene.cpp \
    ../app/src/main/cpp/SystemAbstraction/Application/Tunnel/scene.cpp \
    ../app/src/main/cpp/SystemAbstraction/Application/Tunnel/scene_manager.cpp \
    ../app/src/main/cpp/SystemAbstraction/Application/Tunnel/shader.cpp \
    ../app/src/main/cpp/SystemAbstraction/Application/Tunnel/shape_renderer.cpp \
    ../app/src/main/cpp/SystemAbstraction/Application/Tunnel/tex_quad.cpp \
    ../app/src/main/cpp/SystemAbstraction/Application/Tunnel/text_renderer.cpp \
    ../app/src/main/cpp/SystemAbstraction/Application/Tunnel/texture.cpp \
    ../app/src/main/cpp/SystemAbstraction/Application/Tunnel/ui_scene.cpp \
    ../app/src/main/cpp/SystemAbstraction/Application/Tunnel/util.cpp \
    ../app/src/main/cpp/SystemAbstraction/Application/Tunnel/vertexbuf.cpp \
    ../app/src/main/cpp/SystemAbstraction/Application/Tunnel/welcome_scene.cpp \
    ../app/src/main/cpp/SystemAbstraction/system_abstraction.cpp \
    ../app/src/main/cpp/SystemAbstraction/system_audio.cpp \
    ../app/src/main/cpp/SystemAbstraction/Application/Tunnel/data/alphabet.inl \
    ../app/src/main/cpp/SystemAbstraction/Application/Tunnel/data/ascii_art.inl \
    ../app/src/main/cpp/SystemAbstraction/Application/Tunnel/data/blurb.inl \
    ../app/src/main/cpp/SystemAbstraction/Application/Tunnel/data/cube_geom.inl \
    ../app/src/main/cpp/SystemAbstraction/Application/Tunnel/data/our_shader.inl \
    ../app/src/main/cpp/SystemAbstraction/Application/Tunnel/data/strings.inl \
    ../app/src/main/cpp/SystemAbstraction/Application/Tunnel/data/tunnel_geom.inl

HEADERS += \
    opengl_includes.hpp \
    ../app/src/main/cpp/SystemAbstraction/Application/Tunnel/anim.hpp \
    ../app/src/main/cpp/SystemAbstraction/Application/Tunnel/ascii_to_geom.hpp \
    ../app/src/main/cpp/SystemAbstraction/Application/Tunnel/common.hpp \
    ../app/src/main/cpp/SystemAbstraction/Application/Tunnel/dialog_scene.hpp \
    ../app/src/main/cpp/SystemAbstraction/Application/Tunnel/engine.hpp \
    ../app/src/main/cpp/SystemAbstraction/Application/Tunnel/game_consts.hpp \
    ../app/src/main/cpp/SystemAbstraction/Application/Tunnel/indexbuf.hpp \
    ../app/src/main/cpp/SystemAbstraction/Application/Tunnel/joystick-support.hpp \
    ../app/src/main/cpp/SystemAbstraction/Application/Tunnel/obstacle.hpp \
    ../app/src/main/cpp/SystemAbstraction/Application/Tunnel/obstacle_generator.hpp \
    ../app/src/main/cpp/SystemAbstraction/Application/Tunnel/our_key_codes.hpp \
    ../app/src/main/cpp/SystemAbstraction/Application/Tunnel/our_shader.hpp \
    ../app/src/main/cpp/SystemAbstraction/Application/Tunnel/play_scene.hpp \
    ../app/src/main/cpp/SystemAbstraction/Application/Tunnel/scene.hpp \
    ../app/src/main/cpp/SystemAbstraction/Application/Tunnel/scene_manager.hpp \
    ../app/src/main/cpp/SystemAbstraction/Application/Tunnel/shader.hpp \
    ../app/src/main/cpp/SystemAbstraction/Application/Tunnel/shape_renderer.hpp \
    ../app/src/main/cpp/SystemAbstraction/Application/Tunnel/simplegeom.hpp \
    ../app/src/main/cpp/SystemAbstraction/Application/Tunnel/tex_quad.hpp \
    ../app/src/main/cpp/SystemAbstraction/Application/Tunnel/text_renderer.hpp \
    ../app/src/main/cpp/SystemAbstraction/Application/Tunnel/texture.hpp \
    ../app/src/main/cpp/SystemAbstraction/Application/Tunnel/ui_scene.hpp \
    ../app/src/main/cpp/SystemAbstraction/Application/Tunnel/util.hpp \
    ../app/src/main/cpp/SystemAbstraction/Application/Tunnel/vertexbuf.hpp \
    ../app/src/main/cpp/SystemAbstraction/Application/Tunnel/welcome_scene.hpp \
    ../app/src/main/cpp/SystemAbstraction/system_abstraction.hpp \
    ../app/src/main/cpp/SystemAbstraction/system_audio.hpp \
    ../app/src/main/cpp/SystemAbstraction/system_log.hpp \
    ../app/src/main/cpp/SystemAbstraction/Application/Tunnel/data/design_graffiti_agentorange_www_myfontfree_com.ttf.hpp \
    ../app/src/main/cpp/SystemAbstraction/Application/Tunnel/data/coin_2.png.hpp



