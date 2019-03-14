#pragma once

#include "scene.hpp"
#include "../CapAfri/gui/gui_v2/widget.hpp"
#include "../CapAfri/gui/radio_button.hpp"
#include "../CapAfri/gui/radio_button_manager.hpp"
#include <library_opengles_2/TextRenderer/TextRenderer_v2.hpp>
#include <library_opengles_2/RectangleRenderer/Primitive_Renderer.hpp>
#include <lib_config.hpp>
#include <system_paths.hpp>
#include <OpenSLWrap.hpp>
#include <library_opengles_2/Resources/Resources.hpp>
#include "../CapAfri/PostProcessor.hpp"
#include <sstream>
#include "../CapAfri/game_config.hpp"
#include "../CapAfri/camera_safe_area.hpp"

using namespace std;


class Label : public Widget{
  public:
    Label(GLuint fontSize)
    {
        Resource font_design_graffiti_agentorange("fonts/design_graffiti_agentorange_www_myfontfree_com.ttf");
        // Resource font_design_graffiti_agentorange("fonts/arial.ttf");

        textRenderer_v2 = new TextRenderer_v2(1920, 1080, glm::vec4(1, 0, 0, 1));
        textRenderer_v2->LoadFromMemory("design_graffiti_agentorange_www_myfontfree_com.ttf", font_design_graffiti_agentorange.getData(), font_design_graffiti_agentorange.getSize(), fontSize);
        textRenderer_v2->setColour(glm::vec4(1, 0, 0, 1));
    }

    ~Label()
    {
        if(textRenderer_v2)
            delete textRenderer_v2;
    }

    void CustromDraw() override
    {
        string mText = "LEVEL LOCKED!";
        textRenderer_v2->setCustomPV(mProjection, mView);
        textRenderer_v2->RenderText(1, mText, mGlobalModel, TextRenderer_v2::TEXT_CENTER);
    }

private:
    TextRenderer_v2 * textRenderer_v2 = nullptr;
};

class Explain : public Widget{
  public:
    Explain()
    {
        int height = 1080;
        int width = 1920;

        GLuint fontSize = GLuint(float(height) * 0.06f);
        //Resource font_design_graffiti_agentorange("fonts/design_graffiti_agentorange_www_myfontfree_com.ttf");
        Resource font_ProggyClean("fonts/ProggyClean.ttf");

        textRenderer_v2 = new TextRenderer_v2(width, height, glm::vec4(1, 0, 0, 1));
        textRenderer_v2->LoadFromMemory("ProggyClean.ttf", font_ProggyClean.getData(), font_ProggyClean.getSize(), fontSize);
        textRenderer_v2->setColour(glm::vec4(1, 0, 0, 1));
    }

    ~Explain()
    {
        if(textRenderer_v2)
            delete textRenderer_v2;
    }

    void CustromDraw() override
    {
        string mText = "To play unlock previous\n level or go to store.";
        textRenderer_v2->setCustomPV(mProjection, mView);
        textRenderer_v2->RenderText(1, mText, mGlobalModel, TextRenderer_v2::TEXT_CENTER);
    }

private:
    TextRenderer_v2 * textRenderer_v2 = nullptr;
};


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
    void initMessageBox();
    void initNormalButtons();
    void initRadioButtons();

    const unsigned int LEVELS_COUNT = 3;

    Button buttonPlay;
    Button buttonUnlock;

    RadioButtonManager radioButtonManager;

    RadioButton *button_1;
    Widget * message_widget;
    Widget * label_widget;
    Widget * explain_widget;
    Widget * unlock_widget;
    Widget * cancel_widget;

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

    PostProcessor* mEffects = nullptr;
};
