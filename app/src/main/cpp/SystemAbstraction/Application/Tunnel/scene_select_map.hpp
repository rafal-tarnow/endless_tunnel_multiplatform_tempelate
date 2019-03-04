#pragma once

#include "scene.hpp"
#include "../CapAfri/gui/widget.hpp"
#include "../CapAfri/gui/radio_button.hpp"
#include "../CapAfri/gui/radio_button_manager.hpp"
#include <library_opengles_2/TextRenderer/TextRenderer_v2.hpp>
#include <library_opengles_2/RectangleRenderer/Primitive_Renderer.hpp>
#include <lib_config.hpp>
#include <system_paths.hpp>
#include <OpenSLWrap.hpp>
#include <sstream>
#include "../CapAfri/game_config.hpp"
#include "../CapAfri/camera_safe_area.hpp"

using namespace std;

class SelectMapScene : public Scene, RadioButtonManagerEventLister, ButtonEventListener{
public:
    SelectMapScene();
    ~SelectMapScene();

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
    void initNormalButtons();
    void initRadioButtons();

    const unsigned int LEVELS_COUNT = 3;

    Button buttonPlay;
    Button buttonUnlock;

    RadioButtonManager radioButtonManager;

    RadioButton *button_1;
    Widget * message_button;

    vector<RadioButton *> buttons;

    uint32_t currentMapIndex = 0;

    DE_Rectangle safe_area_background;

    SafeAreaCam safeAreaCam;


    glm::vec2 framebuffer_dimm = glm::vec2(1000,1000);

//    TextRenderer_v2 * textRenderer_v2 = nullptr;

    AudioManager::AudioHandle menuMusicHandle;

    Primitive testPrimitive;

    GameConfig *cfg = GameConfig::getInstance();
    Shader_m * shader;
};
