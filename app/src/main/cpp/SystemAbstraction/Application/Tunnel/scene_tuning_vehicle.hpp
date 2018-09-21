#pragma once

#include "scene.hpp"
#include "../CapAfri/gui/radio_button.hpp"
#include "../CapAfri/gui/radio_button_manager.hpp"
#include <library_opengles_2/TextRenderer/TextRenderer_v2.hpp>
#include <lib_config.hpp>
#include <system_paths.hpp>
#include <sstream>
#include <OpenSLWrap.hpp>
#include "../CapAfri/camera_safe_area.hpp"

using namespace std;

class TuningVehicleScene : public Scene, RadioButtonManagerEventLister, ButtonEventListener{
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


    void RadioButtonManager_onRadioButtonChanged(RadioButton * radioButton);
    void Button_onClicked(Button * button);

private:
    void buttonPlusClicked();
    void buttonMinusClicked();
    void initNormalButtons();
    void initRadioButtons();

    Button buttonPlus;
    Button buttonMinus;
    Button buttonPlay;

    RadioButton * currentRadioButton = nullptr;

    RadioButtonManager radioButtonManager;

    RadioButton button_shockAbsorber;
    RadioButton button_spring;
    RadioButton button_tires;
    RadioButton button_motorTorque;

    DE_Rectangle safe_area_background;

    SafeAreaCam safeAreaCam;


    glm::vec2 framebuffer_dimm = glm::vec2(1000,1000);

    TextRenderer_v2 * textRenderer_v2 = nullptr;

    string configFilePath = getStandardCommonReadWriteDirecory() + "/CapitanAfrica.config";

    LibConfig config;
    float dampingRatio = 0.0f;
    float frequencyHz = 0.0f;
    float maxMotorTorque = 0.0f;
    float friction = 0.0f;

    stringstream stream;

    AudioManager::AudioHandle menuMusicHandle;
};
