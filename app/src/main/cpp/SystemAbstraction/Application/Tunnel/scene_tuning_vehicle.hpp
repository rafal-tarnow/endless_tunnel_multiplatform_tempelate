#pragma once

#include "scene.hpp"
#include "../CapAfri/demo.hpp"
#include "../CapAfri/gui/radio_button.hpp"
#include "../CapAfri/gui/radio_button_manager.hpp"

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
    virtual void OnFramebufferResized(int width, int height);
    virtual void OnJoy(float joyX, float joyY);
    virtual void OnKeyDown(int keyCode);
    virtual void OnPause();

    void gui_onPlayButtonClicked();

private:
    void initButtons();
    RadioButtonManager radioButtonManager;

    RadioButton button_shockAbsorber;
    RadioButton button_spring;
    RadioButton button_tires;

    DE_Rectangle backgroundRect;
    DE_Rectangle safe_area;
    glm::vec2 safe_area_dim;

    glm::vec4 mViewport;
    glm::mat4 mProjection;
    glm::mat4 mView;
};
