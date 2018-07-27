#include "scene_tuning_vehicle.hpp"
#include <system_opengl_include.hpp>
#include "scene_manager.hpp"
#include "play_capafri_scene.hpp"
#include <library_opengles_2/Resources/Resources.hpp>
#include <library_opengles_2/TextureManager/texture_manager.hpp>
#include "welcome_scene.hpp"
#include <iostream>
#include <sstream>
#include <iomanip>

using namespace std;

TuningVehicleScene::TuningVehicleScene()
{
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
    glDisable(GL_DEPTH_TEST);

    glm::vec2 safe_area_dimension = glm::vec2(1920.0f,1080.0f);
    safeAreaCam.setSafeAreaDim(safe_area_dimension);

    DE_initRectangle(&safe_area_background, "textures/garage.png", glm::vec3(safe_area_dimension.x/2.0f, safe_area_dimension.y/2.0f, 0.0f),safe_area_dimension);

    initNormalButtons();
    initRadioButtons();

    config.loadDataFromFileToMemory(configFilePath);
    dampingRatio = config.get_float("dampingRatio");
    frequencyHz = config.get_float("frequencyHz");
    maxMotorTorque = config.get_float("maxMotorTorque");
    friction = config.get_float("friction");

    stream << std::fixed << std::setprecision(1);
}

void TuningVehicleScene::initNormalButtons()
{
    glm::vec3 position = glm::vec3(1920.0f*(4.0f/7.0f),1080.0f*(1.0f/6.0f),0.0f);
    buttonPlus.setPosition(position);
    buttonPlus.setDimm(glm::vec2(138,138));
    buttonPlus.setMatrices(&safeAreaCam.viewport(), &safeAreaCam.projection(), &safeAreaCam.view());
    buttonPlus.setNormalBackgroundTexture(TextureManager::getInstance()->getTextureId("textures/plus_green.png"));
    buttonPlus.setPressedBackgroundTexture(TextureManager::getInstance()->getTextureId("textures/plus_grey.png"));
    buttonPlus.setEventListener(this);

    position = glm::vec3(1920.0f*(3.0f/7.0f),1080.0f*(1.0f/6.0f),0.0f);
    buttonMinus.setPosition(position);
    buttonMinus.setDimm(glm::vec2(138,138));
    buttonMinus.setMatrices(&safeAreaCam.viewport(), &safeAreaCam.projection(), &safeAreaCam.view());
    buttonMinus.setNormalBackgroundTexture(TextureManager::getInstance()->getTextureId("textures/minus_red.png"));
    buttonMinus.setPressedBackgroundTexture(TextureManager::getInstance()->getTextureId("textures/minus_grey.png"));
    buttonMinus.setEventListener(this);

    position = glm::vec3(1920.0f*(6.0f/7.0f),1080.0f*(1.0f/6.0f),0.0f);
    buttonPlay.setPosition(position);
    buttonPlay.setMatrices(&safeAreaCam.viewport(), &safeAreaCam.projection(), &safeAreaCam.view());
    buttonPlay.setNormalBackgroundTexture(TextureManager::getInstance()->getTextureId("textures/shock_absorber.png"));
    buttonPlay.setPressedBackgroundTexture(TextureManager::getInstance()->getTextureId("textures/shock_absorber_pressed.png"));
    buttonPlay.setEventListener(this);
}

void TuningVehicleScene::initRadioButtons()
{

    glm::vec3 position = glm::vec3(1920.0f*(1.0f/4.0f),1080.0f*(2.0f/3.0f),0.0f);

    button_shockAbsorber.setPosition(position);
    button_shockAbsorber.setMatrices(&safeAreaCam.viewport(), &safeAreaCam.projection(), &safeAreaCam.view());
    button_shockAbsorber.setNormalBackgroundTexture(TextureManager::getInstance()->getTextureId("textures/shock_absorber.png"));
    button_shockAbsorber.setPressedBackgroundTexture(TextureManager::getInstance()->getTextureId("textures/shock_absorber_pressed.png"));

    position = glm::vec3(1920.0f*(1.0f/4.0f),1080.0f*(1.0f/3.0f),0.0f);

    button_spring.setPosition(position);
    button_spring.setMatrices(&safeAreaCam.viewport(), &safeAreaCam.projection(), &safeAreaCam.view());
    button_spring.setNormalBackgroundTexture(TextureManager::getInstance()->getTextureId("textures/spring.png"));
    button_spring.setPressedBackgroundTexture(TextureManager::getInstance()->getTextureId("textures/spring_pressed.png"));

    position = glm::vec3(1920.0f*(3.0f/4.0f),1080.0f*(2.0f/3.0f),0.0f);

    button_tires.setPosition(position);
    button_tires.setMatrices(&safeAreaCam.viewport(), &safeAreaCam.projection(), &safeAreaCam.view());
    button_tires.setNormalBackgroundTexture(TextureManager::getInstance()->getTextureId("textures/tires.png"));
    button_tires.setPressedBackgroundTexture(TextureManager::getInstance()->getTextureId("textures/tires_pressed.png"));

    position = glm::vec3(1920.0f*(3.0f/4.0f),1080.0f*(1.0f/3.0f),0.0f);

    button_motorTorque.setPosition(position);
    button_motorTorque.setMatrices(&safeAreaCam.viewport(), &safeAreaCam.projection(), &safeAreaCam.view());
    button_motorTorque.setNormalBackgroundTexture(TextureManager::getInstance()->getTextureId("textures/engine.png"));
    button_motorTorque.setPressedBackgroundTexture(TextureManager::getInstance()->getTextureId("textures/engine_pressed.png"));


    radioButtonManager.addRadioButton(&button_shockAbsorber);
    radioButtonManager.addRadioButton(&button_spring);
    radioButtonManager.addRadioButton(&button_tires);
    radioButtonManager.addRadioButton(&button_motorTorque);

    radioButtonManager.setEventListener(this);


    currentRadioButton = &button_shockAbsorber;
}

TuningVehicleScene::~TuningVehicleScene()
{
    DE_deleteRectangle(&safe_area_background);

    config.set_float("dampingRatio", dampingRatio);
    config.set_float("frequencyHz", frequencyHz);
    config.set_float("maxMotorTorque", maxMotorTorque);
    config.set_float("friction", friction);
    config.saveDataFromMemoryToFile(configFilePath);

    delete textRenderer_v2;
}


void TuningVehicleScene::OnStartGraphics(int width, int height)
{
    framebuffer_dimm = glm::vec2(width, height);

    safeAreaCam.onResize(width, height);

    GLuint fontSize = GLuint(float(height)*0.076f);
    Resource font_design_graffiti_agentorange("fonts/design_graffiti_agentorange_www_myfontfree_com.ttf");
    //Resource font_design_graffiti_agentorange("fonts/arial.ttf");

    textRenderer_v2 = new TextRenderer_v2(width,height, glm::vec4(1,0,0,1));
    textRenderer_v2->LoadFromMemory("Design graffiti agentorange", font_design_graffiti_agentorange.getData(), font_design_graffiti_agentorange.getSize(), fontSize);
}

void TuningVehicleScene::OnKillGraphics()
{

}

void TuningVehicleScene::DoFrame()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    glm::mat4 M_GUI = glm::mat4(1);

    safe_area_background.projection = safeAreaCam.projection();
    safe_area_background.view = safeAreaCam.view();
    safe_area_background.model = M_GUI;

    DE_drawRectangle(&safe_area_background);

    button_shockAbsorber.Render();
    button_spring.Render();
    button_tires.Render();
    button_motorTorque.Render();

    buttonPlus.Render();
    buttonMinus.Render();
    buttonPlay.Render();




    stream.str("");
    if(currentRadioButton == &button_shockAbsorber)
    {
        stream << "Damping " << dampingRatio;
    }
    else if(currentRadioButton == &button_spring)
    {
        stream << "Frequency " << frequencyHz;
    }
    else if(currentRadioButton == &button_motorTorque)
    {
        stream << "MotorTorque " << maxMotorTorque;
    }
    else if(currentRadioButton == &button_tires)
    {
        stream << "Friction " << friction;
    }

    textRenderer_v2->RenderText(stream.str(), framebuffer_dimm.x*0.5, framebuffer_dimm.y*0.46, TextRenderer_v2::TEXT_CENTER);

}

void TuningVehicleScene::buttonPlusClicked()
{
    if(currentRadioButton == &button_shockAbsorber)
    {
        dampingRatio += 0.1f;
    }
    else if(currentRadioButton == &button_spring)
    {
        frequencyHz += 0.1f;
    }
    else if(currentRadioButton == &button_motorTorque)
    {
        maxMotorTorque += 0.1f;
    }
    else if(currentRadioButton == &button_tires)
    {
        friction += 0.1f;
    }
}

void TuningVehicleScene::buttonMinusClicked()
{
    if(currentRadioButton == &button_shockAbsorber)
    {
        dampingRatio -= 0.1f;
    }
    else if(currentRadioButton == &button_spring)
    {
        frequencyHz -= 0.1f;
    }
    else if(currentRadioButton == &button_motorTorque)
    {
        maxMotorTorque -= 0.1f;
    }
    else if(currentRadioButton == &button_tires)
    {
        friction -= 0.1f;
    }
}

void TuningVehicleScene::Button_onClicked(Button * button)
{
    if(button == &buttonPlus)
    {
        buttonPlusClicked();
    }
    else if(button == &buttonMinus)
    {
        buttonMinusClicked();
    }
    else if(button == &buttonPlay)
    {
        SceneManager *mgr = SceneManager::GetInstance();
        mgr->RequestNewScene(new PlayCapAfriScene());
    }
}

void TuningVehicleScene::RadioButtonManager_onRadioButtonChanged(RadioButton * radioButton)
{
    currentRadioButton = radioButton;
}


void TuningVehicleScene::OnPointerDown(int pointerId, const struct PointerCoords *coords)
{
    button_shockAbsorber.onPointerDown(coords->x, coords->y);
    button_spring.onPointerDown(coords->x, coords->y);
    button_tires.onPointerDown(coords->x, coords->y);
    button_motorTorque.onPointerDown(coords->x, coords->y);

    buttonPlus.onPointerDown(coords->x, coords->y);
    buttonMinus.onPointerDown(coords->x, coords->y);
    buttonPlay.onPointerDown(coords->x, coords->y);
}

void TuningVehicleScene::OnPointerUp(int pointerId, const struct PointerCoords *coords)
{
    button_shockAbsorber.onPointerUp();
    button_spring.onPointerUp();
    button_tires.onPointerUp();
    button_motorTorque.onPointerUp();

    buttonPlus.onPointerUp();
    buttonMinus.onPointerUp();
    buttonPlay.onPointerUp();
}

void TuningVehicleScene::OnPointerMove(int pointerId, const struct PointerCoords *coords)
{

}

bool TuningVehicleScene::OnBackKeyPressed()
{
    SceneManager::GetInstance()->RequestNewScene(new WelcomeScene());
}

void TuningVehicleScene::OnFramebufferResized(int width, int height)
{
    framebuffer_dimm = glm::vec2(width, height);

    safeAreaCam.onResize(width, height);
    textRenderer_v2->onVievportResize(width, height);
}

void TuningVehicleScene::OnJoy(float joyX, float joyY)
{

}

void TuningVehicleScene::OnKeyDown(int keyCode)
{

}

void TuningVehicleScene::OnPause()
{

}

