#pragma once

#include "../../system_abstraction.hpp"
#include "../../system_audio.hpp"
#include "engine.hpp"
#include "obstacle_generator.hpp"
#include "obstacle.hpp"
#include "shape_renderer.hpp"
#include "text_renderer.hpp"
#include "util.hpp"
#include <CapAfri/map_editor.hpp>



/* This is the gameplay scene -- the scene that shows the player flying down
 * the infinite tunnel, dodging obstacles, collecting bonuses and being awesome. */
class MapEditorScene : public Scene {
public:
    MapEditorScene();
    virtual void OnStartGraphics(int width, int height);
    virtual void OnKillGraphics();
    virtual void DoFrame();
    virtual void OnPointerDown(int pointerId, const struct PointerCoords *coords);
    virtual void OnPointerUp(int pointerId, const struct PointerCoords *coords);
    virtual void OnScroll(double yoffset);
    virtual void OnPointerMove(int pointerId, const struct PointerCoords *coords);
    virtual bool OnBackKeyPressed();
    virtual void OnScreenResized(int width, int height);
    virtual void OnJoy(float joyX, float joyY);
    virtual void OnKeyDown(int keyCode);
    virtual void OnPause();

protected:
    void RenderMenu();
    void ShowMenu(int menu);
    void HandleMenu(int menuItem);
    void UpdateMenuSelFromTouch(float x, float y);

    MapEditor *mapEditor = nullptr;
    //mapEditor->systemCallback_Scroll(yoffset);
    //mapEditor->systemCallback_keyboard(event, key, 0 , 0);

    unsigned int current_width = 1920;
    unsigned int current_height = 1080;

    // are we showing a menu? If so, what menu?
    static const int MENU_NONE = 0;
    static const int MENU_PAUSE = 1; // pause menu
    static const int MENU_LEVEL = 2; // select starting level
    int mMenu;

    // identifiers for each menu item
    static const int MENUITEM_UNPAUSE = 0;
    static const int MENUITEM_QUIT = 1;
    static const int MENUITEM_START_OVER = 2;
    static const int MENUITEM_RESUME = 3;
    static const int MENUITEM_COUNT = 4;

    // text for each menu item
    const char *mMenuItemText[MENUITEM_COUNT];

    // menu items on current menu
    static const int MENUITEMS_MAX = 4;
    int mMenuItems[MENUITEMS_MAX];
    int mMenuItemCount; // # of menu items
    int mMenuSel; // index of selected menu item

    // is user touching the screen to select menu? are they using the buttons?
    bool mMenuTouchActive;

    TrivialShader *mmTrivialShader;
    ShapeRenderer *mmShapeRenderer;
    TextRenderer *mmTextRenderer;
};



