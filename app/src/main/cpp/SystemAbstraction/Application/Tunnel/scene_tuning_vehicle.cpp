#include "scene_tuning_vehicle.hpp"
#include <system_opengl_include.hpp>
#include "scene_manager.hpp"
#include "play_capafri_scene.hpp"
#include <library_opengles_2/Resources/Resources.hpp>
#include <library_opengles_2/TextureManager/texture_manager.hpp>
#include "welcome_scene.hpp"
#include <iostream>
#include <sstream>

using namespace std;

TuningVehicleScene::TuningVehicleScene()
{
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
    glDisable(GL_DEPTH_TEST);


    safe_area_dim = glm::vec2(1920.0f,1080.0f);
    DE_initRectangle(&safe_area, "textures/garage.png", glm::vec3(safe_area_dim.x/2.0f, safe_area_dim.y/2.0f, 0.0f),safe_area_dim);

    DE_initRectangle(&backgroundRect, "textures/bg.png", glm::vec3(0.0f,0.0f,0.0f),safe_area_dim);

    initNormalButtons();
    initRadioButtons();


    config.loadDataFromFileToMemory(configFilePath);
    dampingRatio = config.get_float("dampingRatio");
    frequencyHz = config.get_float("frequencyHz");
    maxMotorTorque = config.get_float("maxMotorTorque");
    friction = config.get_float("friction");
}

void TuningVehicleScene::initNormalButtons()
{
    glm::vec3 position = glm::vec3(1920.0f*(4.0f/7.0f),1080.0f*(1.0f/6.0f),0.0f);
    buttonPlus.setPosition(position);
    buttonPlus.setDimm(glm::vec2(138,138));
    buttonPlus.setMatrices(&mViewport, &mProjection, &mView);
    buttonPlus.setNormalBackgroundTexture(TextureManager::getTextureId("textures/plus_green.png"));
    buttonPlus.setPressedBackgroundTexture(TextureManager::getTextureId("textures/plus_grey.png"));
    buttonPlus.setEventListener(this);

    position = glm::vec3(1920.0f*(3.0f/7.0f),1080.0f*(1.0f/6.0f),0.0f);
    buttonMinus.setPosition(position);
    buttonMinus.setDimm(glm::vec2(138,138));
    buttonMinus.setMatrices(&mViewport, &mProjection, &mView);
    buttonMinus.setNormalBackgroundTexture(TextureManager::getTextureId("textures/minus_red.png"));
    buttonMinus.setPressedBackgroundTexture(TextureManager::getTextureId("textures/minus_grey.png"));
    buttonMinus.setEventListener(this);

    position = glm::vec3(1920.0f*(6.0f/7.0f),1080.0f*(1.0f/6.0f),0.0f);
    buttonPlay.setPosition(position);
    buttonPlay.setMatrices(&mViewport, &mProjection, &mView);
    buttonPlay.setNormalBackgroundTexture(TextureManager::getTextureId("textures/shock_absorber.png"));
    buttonPlay.setPressedBackgroundTexture(TextureManager::getTextureId("textures/shock_absorber_pressed.png"));
    buttonPlay.setEventListener(this);
}

void TuningVehicleScene::initRadioButtons()
{

    glm::vec3 position = glm::vec3(1920.0f*(1.0f/4.0f),1080.0f*(2.0f/3.0f),0.0f);

    button_shockAbsorber.setPosition(position);
    button_shockAbsorber.setMatrices(&mViewport, &mProjection, &mView);
    button_shockAbsorber.setNormalBackgroundTexture(TextureManager::getTextureId("textures/shock_absorber.png"));
    button_shockAbsorber.setPressedBackgroundTexture(TextureManager::getTextureId("textures/shock_absorber_pressed.png"));

    position = glm::vec3(1920.0f*(1.0f/4.0f),1080.0f*(1.0f/3.0f),0.0f);

    button_spring.setPosition(position);
    button_spring.setMatrices(&mViewport, &mProjection, &mView);
    button_spring.setNormalBackgroundTexture(TextureManager::getTextureId("textures/spring.png"));
    button_spring.setPressedBackgroundTexture(TextureManager::getTextureId("textures/spring_pressed.png"));

    position = glm::vec3(1920.0f*(3.0f/4.0f),1080.0f*(2.0f/3.0f),0.0f);

    button_tires.setPosition(position);
    button_tires.setMatrices(&mViewport, &mProjection, &mView);
    button_tires.setNormalBackgroundTexture(TextureManager::getTextureId("textures/tires.png"));
    button_tires.setPressedBackgroundTexture(TextureManager::getTextureId("textures/tires_pressed.png"));

    position = glm::vec3(1920.0f*(3.0f/4.0f),1080.0f*(1.0f/3.0f),0.0f);

    button_motorTorque.setPosition(position);
    button_motorTorque.setMatrices(&mViewport, &mProjection, &mView);
    button_motorTorque.setNormalBackgroundTexture(TextureManager::getTextureId("textures/engine.png"));
    button_motorTorque.setPressedBackgroundTexture(TextureManager::getTextureId("textures/engine_pressed.png"));


    radioButtonManager.addRadioButton(&button_shockAbsorber);
    radioButtonManager.addRadioButton(&button_spring);
    radioButtonManager.addRadioButton(&button_tires);
    radioButtonManager.addRadioButton(&button_motorTorque);

    radioButtonManager.setEventListener(this);


    currentRadioButton = &button_shockAbsorber;
}

TuningVehicleScene::~TuningVehicleScene()
{
    DE_deleteRectangle(&safe_area);
    DE_deleteRectangle(&backgroundRect);

    config.set_float("dampingRatio", dampingRatio);
    config.set_float("frequencyHz", frequencyHz);
    config.set_float("maxMotorTorque", maxMotorTorque);
    config.set_float("friction", friction);
    config.saveDataFromMemoryToFile(configFilePath);

    delete textRenderer_v2;
    delete textRenderer_v2_second;
}


void TuningVehicleScene::OnStartGraphics(int width, int height)
{
    mViewport = glm::vec4(0,0,width,height);

    GLuint fontSize = GLuint(float(height)*0.076f);
    Resource font_design_graffiti_agentorange("fonts/design_graffiti_agentorange_www_myfontfree_com.ttf");

    textRenderer_v2 = new TextRenderer_v2(width,height, glm::vec4(1,0,0,1));
    textRenderer_v2->LoadFromMemory(font_design_graffiti_agentorange.getData(), font_design_graffiti_agentorange.getSize(), fontSize);

    textRenderer_v2_second = new TextRenderer_v2(width,height, glm::vec4(0,1,0,1));
    textRenderer_v2_second->LoadFromMemory(font_design_graffiti_agentorange.getData(), font_design_graffiti_agentorange.getSize(), fontSize*(2.0f/3.0f));
}

void TuningVehicleScene::OnKillGraphics()
{

}

void TuningVehicleScene::DoFrame()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    //  vehicleTuningGui_render(framebuffer_width, framebuffet_height);


    auto framebuffer_aspect = float(mViewport.z)/float(mViewport.w);
    auto safe_area_aspect = safe_area_dim.x/safe_area_dim.y;



    if(safe_area_aspect > framebuffer_aspect)
    {
        auto half_width = safe_area_dim.x/2.0f;
        auto half_height = half_width/framebuffer_aspect;
        mProjection = glm::ortho(-half_width, half_width, -half_height, half_height, -1000.0f, 1000.0f);
    }
    else
    {
        auto half_height = safe_area_dim.y/2.0f;
        auto half_width = half_height*framebuffer_aspect;
        mProjection = glm::ortho(-half_width, half_width, -half_height, half_height, -1000.0f, 1000.0f);
    }
    //    glm::mat4 V_GUI = glm::lookAt(glm::vec3(float(framebuffer_width)/2.0f, float(framebuffet_height)/2.0f, 1.0f),glm::vec3(float(framebuffer_width)/2.0f, float(framebuffet_height)/2.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    //    glm::mat4 M_GUI = glm::translate(glm::mat4(1),glm::vec3(1920/2, 0,0));

    //glm::mat4 P_GUI = glm::mat4(1);
    mView = glm::lookAt(glm::vec3(safe_area_dim.x/2.0f, safe_area_dim.y/2.0f, 1.0f), glm::vec3(safe_area_dim.x/2.0f, safe_area_dim.y/2.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 M_GUI = glm::mat4(1);


    backgroundRect.projection = mProjection;
    backgroundRect.view = mView;
    backgroundRect.model = M_GUI;
    //DE_drawRectangle(&backgroundRect);


    safe_area.projection = mProjection;
    safe_area.view = mView;
    safe_area.model = M_GUI;
    DE_drawRectangle(&safe_area);



    button_shockAbsorber.Render();
    button_spring.Render();
    button_tires.Render();
    button_motorTorque.Render();

    buttonPlus.Render();
    buttonMinus.Render();
    buttonPlay.Render();



    stringstream stream;

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

    textRenderer_v2->RenderText(stream.str(), mViewport.z*0.3, mViewport.w*0.46);
    //textRenderer_v2_second->RenderText(stream.str(), mViewport.z*0.3, mViewport.w*0.46);


    glFlush();
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
    mViewport = glm::vec4(0,0,width,height);

    textRenderer_v2->onVievportResize(mViewport.z, mViewport.w);
    textRenderer_v2_second->onVievportResize(mViewport.z, mViewport.w);
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

