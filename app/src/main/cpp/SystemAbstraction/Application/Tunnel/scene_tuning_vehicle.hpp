#pragma once

#include "scene.hpp"
#include "../CapAfri/demo.hpp"

class TuningVehicleScene : public Scene, VehicleTuningGuiEventListener {
public:
    TuningVehicleScene();
    ~TuningVehicleScene();

    virtual void OnStartGraphics(int width, int height);
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

    void gui_onPlayButtonClicked();

private:
    int framebuffer_width;
    int framebuffet_height;

};