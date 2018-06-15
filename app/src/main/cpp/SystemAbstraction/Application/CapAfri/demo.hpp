#pragma once

#include "./Application/libs/nuklear/implementation/nuklear_gles2.hpp"
#include "system_abstraction.hpp"

#define UNUSED(a) (void)a
#define MAX(a,b) ((a) < (b) ? (b) : (a))


class MapEditorGuiEventListener{
public:

    virtual void gui_onSaveMapButtonClicked() = 0;
    virtual void gui_onClearMapButtonClicked() = 0;
    virtual void gui_onCursorModeChanged(int mode) = 0;
    virtual void gui_onCurrentMapChanged(unsigned int currentMap) = 0;
};

enum theme {THEME_BLACK, THEME_WHITE, THEME_RED, THEME_BLUE, THEME_DARK};
void set_style(struct nk_context *ctx, enum theme theme);

void demo_init(int width, int height);
struct nk_context * demo_getContext();

void demo_onCharCallback(unsigned int codepoint);
void demo_onScrollCallback(double yoffset);
void demo_onMouseButtonCallback(SystemAbstraction::MouseButton mouseButton, SystemAbstraction::ButtonEvent event, int x, int y);
void demo_onMouseMoveCallcack(int x, int y);
void demo_onPointerMoveCallback(int pointerId, const struct PointerCoords *coords);
void demo_onKeyCallback(SystemAbstraction::ButtonEvent event, SystemAbstraction::Key key, SystemAbstraction::Mods mods, int x, int y);

int mapEditorGui_isAnyWindowHovered();
void mapEditorGui_setEventListener(MapEditorGuiEventListener *eventListener);
void mapEditorGui_render(int fb_width, int fb_height);

void demo_uninit();
