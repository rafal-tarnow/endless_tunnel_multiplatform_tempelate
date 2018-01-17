TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

LIBS += -lGLESv2
LIBS += -lglfw


SOURCES += main.cpp \
    ../app/src/main/cpp/SystemAbstraction/Game/data/alphabet.inl \
    ../app/src/main/cpp/SystemAbstraction/Game/data/ascii_art.inl \
    ../app/src/main/cpp/SystemAbstraction/Game/data/blurb.inl \
    ../app/src/main/cpp/SystemAbstraction/Game/data/cube_geom.inl \
    ../app/src/main/cpp/SystemAbstraction/Game/data/our_shader.inl \
    ../app/src/main/cpp/SystemAbstraction/Game/data/strings.inl \
    ../app/src/main/cpp/SystemAbstraction/Game/data/tunnel_geom.inl \
    ../app/src/main/cpp/SystemAbstraction/Game/anim.cpp \
    ../app/src/main/cpp/SystemAbstraction/Game/ascii_to_geom.cpp \
    ../app/src/main/cpp/SystemAbstraction/Game/dialog_scene.cpp \
    ../app/src/main/cpp/SystemAbstraction/Game/indexbuf.cpp \
    ../app/src/main/cpp/SystemAbstraction/Game/obstacle.cpp \
    ../app/src/main/cpp/SystemAbstraction/Game/obstacle_generator.cpp \
    ../app/src/main/cpp/SystemAbstraction/Game/our_shader.cpp \
    ../app/src/main/cpp/SystemAbstraction/Game/play_scene.cpp \
    ../app/src/main/cpp/SystemAbstraction/Game/scene.cpp \
    ../app/src/main/cpp/SystemAbstraction/Game/scene_manager.cpp \
    ../app/src/main/cpp/SystemAbstraction/Game/shader.cpp \
    ../app/src/main/cpp/SystemAbstraction/Game/shape_renderer.cpp \
    ../app/src/main/cpp/SystemAbstraction/Game/tex_quad.cpp \
    ../app/src/main/cpp/SystemAbstraction/Game/text_renderer.cpp \
    ../app/src/main/cpp/SystemAbstraction/Game/texture.cpp \
    ../app/src/main/cpp/SystemAbstraction/Game/ui_scene.cpp \
    ../app/src/main/cpp/SystemAbstraction/Game/util.cpp \
    ../app/src/main/cpp/SystemAbstraction/Game/vertexbuf.cpp \
    ../app/src/main/cpp/SystemAbstraction/Game/welcome_scene.cpp \
    ../app/src/main/cpp/SystemAbstraction/system_abstraction.cpp \
    ../app/src/main/cpp/SystemAbstraction/system_audio.cpp

HEADERS += \
    opengl_includes.hpp \
    ../app/src/main/cpp/SystemAbstraction/Game/anim.hpp \
    ../app/src/main/cpp/SystemAbstraction/Game/ascii_to_geom.hpp \
    ../app/src/main/cpp/SystemAbstraction/Game/common.hpp \
    ../app/src/main/cpp/SystemAbstraction/Game/dialog_scene.hpp \
    ../app/src/main/cpp/SystemAbstraction/Game/engine.hpp \
    ../app/src/main/cpp/SystemAbstraction/Game/game_consts.hpp \
    ../app/src/main/cpp/SystemAbstraction/Game/indexbuf.hpp \
    ../app/src/main/cpp/SystemAbstraction/Game/joystick-support.hpp \
    ../app/src/main/cpp/SystemAbstraction/Game/obstacle.hpp \
    ../app/src/main/cpp/SystemAbstraction/Game/obstacle_generator.hpp \
    ../app/src/main/cpp/SystemAbstraction/Game/our_key_codes.hpp \
    ../app/src/main/cpp/SystemAbstraction/Game/our_shader.hpp \
    ../app/src/main/cpp/SystemAbstraction/Game/play_scene.hpp \
    ../app/src/main/cpp/SystemAbstraction/Game/scene.hpp \
    ../app/src/main/cpp/SystemAbstraction/Game/scene_manager.hpp \
    ../app/src/main/cpp/SystemAbstraction/Game/shader.hpp \
    ../app/src/main/cpp/SystemAbstraction/Game/shape_renderer.hpp \
    ../app/src/main/cpp/SystemAbstraction/Game/simplegeom.hpp \
    ../app/src/main/cpp/SystemAbstraction/Game/tex_quad.hpp \
    ../app/src/main/cpp/SystemAbstraction/Game/text_renderer.hpp \
    ../app/src/main/cpp/SystemAbstraction/Game/texture.hpp \
    ../app/src/main/cpp/SystemAbstraction/Game/ui_scene.hpp \
    ../app/src/main/cpp/SystemAbstraction/Game/util.hpp \
    ../app/src/main/cpp/SystemAbstraction/Game/vertexbuf.hpp \
    ../app/src/main/cpp/SystemAbstraction/Game/welcome_scene.hpp \
    ../app/src/main/cpp/SystemAbstraction/system_abstraction.hpp \
    ../app/src/main/cpp/SystemAbstraction/system_audio.hpp \
    ../app/src/main/cpp/SystemAbstraction/system_log.hpp

