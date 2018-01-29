DEFINES += GLM_FORCE_SIZE_T_LENGTH
DEFINES += GLM_FORCE_RADIANS

include(../app/src/main/cpp/SystemAbstraction/Application/libs/SOIL/soil_qt.pri)
include(../app/src/main/cpp/SystemAbstraction/Application/libs/glm/glm_qt.pri)

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
    ../app/src/main/cpp/SystemAbstraction/Application/anim.cpp \
    ../app/src/main/cpp/SystemAbstraction/Application/ascii_to_geom.cpp \
    ../app/src/main/cpp/SystemAbstraction/Application/dialog_scene.cpp \
    ../app/src/main/cpp/SystemAbstraction/Application/indexbuf.cpp \
    ../app/src/main/cpp/SystemAbstraction/Application/obstacle.cpp \
    ../app/src/main/cpp/SystemAbstraction/Application/obstacle_generator.cpp \
    ../app/src/main/cpp/SystemAbstraction/Application/our_shader.cpp \
    ../app/src/main/cpp/SystemAbstraction/Application/play_scene.cpp \
    ../app/src/main/cpp/SystemAbstraction/Application/scene.cpp \
    ../app/src/main/cpp/SystemAbstraction/Application/scene_manager.cpp \
    ../app/src/main/cpp/SystemAbstraction/Application/shader.cpp \
    ../app/src/main/cpp/SystemAbstraction/Application/shape_renderer.cpp \
    ../app/src/main/cpp/SystemAbstraction/Application/tex_quad.cpp \
    ../app/src/main/cpp/SystemAbstraction/Application/text_renderer.cpp \
    ../app/src/main/cpp/SystemAbstraction/Application/texture.cpp \
    ../app/src/main/cpp/SystemAbstraction/Application/ui_scene.cpp \
    ../app/src/main/cpp/SystemAbstraction/Application/util.cpp \
    ../app/src/main/cpp/SystemAbstraction/Application/vertexbuf.cpp \
    ../app/src/main/cpp/SystemAbstraction/Application/welcome_scene.cpp \
    ../app/src/main/cpp/SystemAbstraction/system_abstraction.cpp \
    ../app/src/main/cpp/SystemAbstraction/system_audio.cpp \
    ../app/src/main/cpp/SystemAbstraction/Application/data/alphabet.inl \
    ../app/src/main/cpp/SystemAbstraction/Application/data/ascii_art.inl \
    ../app/src/main/cpp/SystemAbstraction/Application/data/blurb.inl \
    ../app/src/main/cpp/SystemAbstraction/Application/data/cube_geom.inl \
    ../app/src/main/cpp/SystemAbstraction/Application/data/our_shader.inl \
    ../app/src/main/cpp/SystemAbstraction/Application/data/strings.inl \
    ../app/src/main/cpp/SystemAbstraction/Application/data/tunnel_geom.inl

HEADERS += \
    opengl_includes.hpp \
    ../app/src/main/cpp/SystemAbstraction/Application/anim.hpp \
    ../app/src/main/cpp/SystemAbstraction/Application/ascii_to_geom.hpp \
    ../app/src/main/cpp/SystemAbstraction/Application/common.hpp \
    ../app/src/main/cpp/SystemAbstraction/Application/dialog_scene.hpp \
    ../app/src/main/cpp/SystemAbstraction/Application/engine.hpp \
    ../app/src/main/cpp/SystemAbstraction/Application/game_consts.hpp \
    ../app/src/main/cpp/SystemAbstraction/Application/indexbuf.hpp \
    ../app/src/main/cpp/SystemAbstraction/Application/joystick-support.hpp \
    ../app/src/main/cpp/SystemAbstraction/Application/obstacle.hpp \
    ../app/src/main/cpp/SystemAbstraction/Application/obstacle_generator.hpp \
    ../app/src/main/cpp/SystemAbstraction/Application/our_key_codes.hpp \
    ../app/src/main/cpp/SystemAbstraction/Application/our_shader.hpp \
    ../app/src/main/cpp/SystemAbstraction/Application/play_scene.hpp \
    ../app/src/main/cpp/SystemAbstraction/Application/scene.hpp \
    ../app/src/main/cpp/SystemAbstraction/Application/scene_manager.hpp \
    ../app/src/main/cpp/SystemAbstraction/Application/shader.hpp \
    ../app/src/main/cpp/SystemAbstraction/Application/shape_renderer.hpp \
    ../app/src/main/cpp/SystemAbstraction/Application/simplegeom.hpp \
    ../app/src/main/cpp/SystemAbstraction/Application/tex_quad.hpp \
    ../app/src/main/cpp/SystemAbstraction/Application/text_renderer.hpp \
    ../app/src/main/cpp/SystemAbstraction/Application/texture.hpp \
    ../app/src/main/cpp/SystemAbstraction/Application/ui_scene.hpp \
    ../app/src/main/cpp/SystemAbstraction/Application/util.hpp \
    ../app/src/main/cpp/SystemAbstraction/Application/vertexbuf.hpp \
    ../app/src/main/cpp/SystemAbstraction/Application/welcome_scene.hpp \
    ../app/src/main/cpp/SystemAbstraction/system_abstraction.hpp \
    ../app/src/main/cpp/SystemAbstraction/system_audio.hpp \
    ../app/src/main/cpp/SystemAbstraction/system_log.hpp



