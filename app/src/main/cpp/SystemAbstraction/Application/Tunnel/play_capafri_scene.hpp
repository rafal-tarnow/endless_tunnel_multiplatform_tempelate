#pragma once

#include "../../system_abstraction.hpp"
#include "../../system_audio.hpp"
#include "engine.hpp"
#include "obstacle_generator.hpp"
#include "obstacle.hpp"
#include "shape_renderer.hpp"
#include "text_renderer.hpp"
#include "util.hpp"
#include <CapAfri/game.hpp>



/* This is the gameplay scene -- the scene that shows the player flying down
 * the infinite tunnel, dodging obstacles, collecting bonuses and being awesome. */
class PlayCapAfriScene : public Scene {
public:
    PlayCapAfriScene();
    virtual void OnStartGraphics();
    virtual void OnKillGraphics();
    virtual void DoFrame();
    virtual void OnPointerDown(int pointerId, const struct PointerCoords *coords);
    virtual void OnPointerUp(int pointerId, const struct PointerCoords *coords);
    virtual void OnPointerMove(int pointerId, const struct PointerCoords *coords);
    virtual bool OnBackKeyPressed();
    virtual void OnScreenResized(int width, int height);
    virtual void OnJoy(float joyX, float joyY);
    virtual void OnKeyDown(int keyCode);
    virtual void OnPause();

protected:
    Game *game = nullptr;
    unsigned int current_width = 1920;
    unsigned int current_height = 1080;


};



