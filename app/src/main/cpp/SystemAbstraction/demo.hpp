#pragma once

#include "./Application/libs/nuklear/implementation/nuklear_gles2.hpp"
#include "system_abstraction.hpp"

#define UNUSED(a) (void)a
#define MAX(a,b) ((a) < (b) ? (b) : (a))

struct media {
    struct nk_font *font_14;
    struct nk_font *font_18;
    struct nk_font *font_20;
    struct nk_font *font_22;

    struct nk_image unchecked;
    struct nk_image checked;
    struct nk_image rocket;
    struct nk_image cloud;
    struct nk_image pen;
    struct nk_image play;
    struct nk_image pause;
    struct nk_image stop;
    struct nk_image prev;
    struct nk_image next;
    struct nk_image tools;
    struct nk_image dir;
    struct nk_image copy;
    struct nk_image convert;
    struct nk_image del;
    struct nk_image edit;
    struct nk_image images[9];
    struct nk_image menu[6];
};

void demo_init(int width, int height);

void demo_onCharCallback(unsigned int codepoint);
void demo_onPointerMoveCallback(int pointerId, const struct PointerCoords *coords);
void demo_onScrollCallback(double yoffset);
void demo_onMouseButtonCallback(SystemAbstraction::MouseButton mouseButton, SystemAbstraction::ButtonEvent event, int x, int y);
void demo_onKeyCallback(SystemAbstraction::ButtonEvent event, SystemAbstraction::Key key, SystemAbstraction::Mods mods, int x, int y);

void demo_render(int fb_width, int fb_height);

void demo_uninit();
