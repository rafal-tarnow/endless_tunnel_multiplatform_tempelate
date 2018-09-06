#include "scene_select_map.hpp"
#include <system_opengl_include.hpp>
#include "scene_manager.hpp"
#include "scene_tuning_vehicle.hpp"
#include "play_capafri_scene.hpp"
#include <library_opengles_2/Resources/Resources.hpp>
#include <library_opengles_2/TextureManager/texture_manager.hpp>
#include "welcome_scene.hpp"
#include <iostream>
#include <sstream>
#include <iomanip>

using namespace std;

SelectMapScene::SelectMapScene()
{
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
    glDisable(GL_DEPTH_TEST);

    glm::vec2 safe_area_dimension = glm::vec2(1920.0f,1080.0f);
    safeAreaCam.setSafeAreaDim(safe_area_dimension);

    DE_initRectangle(&safe_area_background, "textures/select_map.png", glm::vec3(safe_area_dimension.x/2.0f, safe_area_dimension.y/2.0f, 0.0f),safe_area_dimension);

    initNormalButtons();
    initRadioButtons();

    config.loadDataFromFileToMemory(configFilePath);
    dampingRatio = config.get_float("dampingRatio");
    frequencyHz = config.get_float("frequencyHz");
    maxMotorTorque = config.get_float("maxMotorTorque");
    friction = config.get_float("friction");

    stream << std::fixed << std::setprecision(1);

    vector<glm::vec3> verticles;
    verticles.push_back(glm::vec3(0,0,0));
    verticles.push_back(glm::vec3(1920,0,0));
    verticles.push_back(glm::vec3(1920,1080,0));
    verticles.push_back(glm::vec3(0,1080,0));
    PR_init(&testPrimitive, verticles.data(), 4,glm::vec4(1.0, 0.0, 0.0, 1.0), GL_TRIANGLE_FAN, GL_DYNAMIC_DRAW);
}

void SelectMapScene::initNormalButtons()
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

void SelectMapScene::initRadioButtons()
{

    glm::vec3 position = glm::vec3(1920.0f*(1.0f/4.0f),1080.0f*(2.0f/3.0f),0.0f);

    button_shockAbsorber.setText("1");
    button_shockAbsorber.setModel(glm::translate(glm::mat4(1),position));
    button_shockAbsorber.setMatrices(&safeAreaCam.viewport(), &safeAreaCam.projection(), &safeAreaCam.view());
    button_shockAbsorber.setDimm(glm::vec2(200,200));
    button_shockAbsorber.setNormalBackgroundTexture(TextureManager::getInstance()->getTextureId("textures/level_icon.png"));
    button_shockAbsorber.setPressedBackgroundTexture(TextureManager::getInstance()->getTextureId("textures/level_icon_mark.png"));

    position = glm::vec3(1920.0f*(1.0f/4.0f),1080.0f*(1.0f/3.0f),0.0f);

    button_spring.setText("1");
    button_spring.setModel(glm::translate(glm::mat4(1),position));
    button_spring.setMatrices(&safeAreaCam.viewport(), &safeAreaCam.projection(), &safeAreaCam.view());
    button_spring.setDimm(glm::vec2(200,200));
    button_spring.setNormalBackgroundTexture(TextureManager::getInstance()->getTextureId("textures/level_icon.png"));
    button_spring.setPressedBackgroundTexture(TextureManager::getInstance()->getTextureId("textures/level_icon_mark.png"));

    position = glm::vec3(1920.0f*(3.0f/4.0f),1080.0f*(2.0f/3.0f),0.0f);

    button_tires.setText("1");
    button_tires.setModel(glm::translate(glm::mat4(1),position));
    button_tires.setMatrices(&safeAreaCam.viewport(), &safeAreaCam.projection(), &safeAreaCam.view());
    button_tires.setDimm(glm::vec2(200,200));
    button_tires.setNormalBackgroundTexture(TextureManager::getInstance()->getTextureId("textures/level_icon.png"));
    button_tires.setPressedBackgroundTexture(TextureManager::getInstance()->getTextureId("textures/level_icon_mark.png"));

    position = glm::vec3(1920.0f*(3.0f/4.0f),1080.0f*(1.0f/3.0f),0.0f);

    button_motorTorque.setText("1");
    button_motorTorque.setModel(glm::translate(glm::mat4(1),position));
    button_motorTorque.setMatrices(&safeAreaCam.viewport(), &safeAreaCam.projection(), &safeAreaCam.view());
    button_motorTorque.setDimm(glm::vec2(200,200));
    button_motorTorque.setNormalBackgroundTexture(TextureManager::getInstance()->getTextureId("textures/level_icon.png"));
    button_motorTorque.setPressedBackgroundTexture(TextureManager::getInstance()->getTextureId("textures/level_icon_mark.png"));


    radioButtonManager.addRadioButton(&button_shockAbsorber);
    radioButtonManager.addRadioButton(&button_spring);
    radioButtonManager.addRadioButton(&button_tires);
    radioButtonManager.addRadioButton(&button_motorTorque);

    radioButtonManager.setEventListener(this);


    currentRadioButton = &button_shockAbsorber;
}

SelectMapScene::~SelectMapScene()
{
    DE_deleteRectangle(&safe_area_background);

    config.set_float("dampingRatio", dampingRatio);
    config.set_float("frequencyHz", frequencyHz);
    config.set_float("maxMotorTorque", maxMotorTorque);
    config.set_float("friction", friction);
    config.saveDataFromMemoryToFile(configFilePath);

    PR_delete(&testPrimitive);

    delete textRenderer_v2;
}


void SelectMapScene::OnStartGraphics(int width, int height)
{
    framebuffer_dimm = glm::vec2(width, height);

    safeAreaCam.onResize(width, height);

    GLuint fontSize = GLuint(float(height)*0.076f);
    Resource font_design_graffiti_agentorange("fonts/design_graffiti_agentorange_www_myfontfree_com.ttf");
    //Resource font_design_graffiti_agentorange("fonts/arial.ttf");

    textRenderer_v2 = new TextRenderer_v2(width,height, glm::vec4(1,0,0,1));
    textRenderer_v2->LoadFromMemory("Design graffiti agentorange", font_design_graffiti_agentorange.getData(), font_design_graffiti_agentorange.getSize(), fontSize);
}

void SelectMapScene::OnKillGraphics()
{

}

void SelectMapScene::DoFrame()
{
    //glEnable(GL_STENCIL_TEST);
    glStencilFunc(GL_ALWAYS, 1, 0xFF);
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    static float x_pos = 0;
    x_pos += 0.5f;


    glm::mat4 all_model = glm::translate(glm::mat4(1),glm::vec3(200,1080/2,0));


    testPrimitive.model = glm::mat4(1);


    glStencilMask(0xFF);
    glClear(GL_STENCIL_BUFFER_BIT);

    PR_setColour(&testPrimitive, glm::vec4(0.7,0.7,0.7,1.0));

    vector<glm::vec3> verticles;
    verticles.push_back(glm::vec3(0,0,0));
    verticles.push_back(glm::vec3(1920,0,0));
    verticles.push_back(glm::vec3(1920,1080,0));
    verticles.push_back(glm::vec3(0,1080,0));
    PR_setVerticles(&testPrimitive, verticles.data(), 4);

    PR_draw(&testPrimitive, 1.0);

    glStencilMask(0x00);
    glStencilFunc(GL_EQUAL, 1, 0xFF);

    verticles.clear();
    verticles.push_back(glm::vec3(-200,200,0));
    verticles.push_back(glm::vec3(-200,-200,0));
    verticles.push_back(glm::vec3(200,-200,0));
    verticles.push_back(glm::vec3(200,200,0));
    PR_setVerticles(&testPrimitive, verticles.data(), 4);
    PR_setColour(&testPrimitive, glm::vec4(1.0,0.0,0.0,1.0));






    glm::mat4 model;
    model = glm::translate(glm::mat4(1),glm::vec3(0,0,0));

    button_spring.setModel(all_model*model);

    model = glm::translate(model,glm::vec3(400,0,0));

    button_tires.setModel(all_model*model);

    model = glm::translate(model,glm::vec3(400,0,0));

    button_motorTorque.setModel(all_model*model);

    model = glm::translate(model,glm::vec3(400,0,0));
    button_shockAbsorber.setModel(all_model*model);

    button_shockAbsorber.Render();
    button_spring.Render();
    button_tires.Render();
    button_motorTorque.Render();


    glm::mat4 M_GUI = glm::mat4(1);

    safe_area_background.projection = safeAreaCam.projection();
    safe_area_background.view = safeAreaCam.view();
    safe_area_background.model = M_GUI;


    DE_drawRectangle(&safe_area_background);



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



    textRenderer_v2->RenderText(stream.str(), (framebuffer_dimm.x*0.5) + x_pos, framebuffer_dimm.y*0.46, TextRenderer_v2::TEXT_CENTER);

    testPrimitive.projection = safeAreaCam.projection();
    testPrimitive.view = safeAreaCam.view();

    //glDisable(GL_STENCIL_TEST);


}

void SelectMapScene::buttonPlusClicked()
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

void SelectMapScene::buttonMinusClicked()
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

void SelectMapScene::Button_onClicked(Button * button)
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
        mgr->RequestNewScene(new TuningVehicleScene());
    }
}

void SelectMapScene::RadioButtonManager_onRadioButtonChanged(RadioButton * radioButton)
{
    currentRadioButton = radioButton;
}


void SelectMapScene::OnPointerDown(int pointerId, const struct PointerCoords *coords)
{
    button_shockAbsorber.onPointerDown(coords->x, coords->y);
    button_spring.onPointerDown(coords->x, coords->y);
    button_tires.onPointerDown(coords->x, coords->y);
    button_motorTorque.onPointerDown(coords->x, coords->y);

    buttonPlus.onPointerDown(coords->x, coords->y);
    buttonMinus.onPointerDown(coords->x, coords->y);
    buttonPlay.onPointerDown(coords->x, coords->y);
}

void SelectMapScene::OnPointerUp(int pointerId, const struct PointerCoords *coords)
{
    button_shockAbsorber.onPointerUp();
    button_spring.onPointerUp();
    button_tires.onPointerUp();
    button_motorTorque.onPointerUp();

    buttonPlus.onPointerUp();
    buttonMinus.onPointerUp();
    buttonPlay.onPointerUp();
}

void SelectMapScene::OnPointerMove(int pointerId, const struct PointerCoords *coords)
{

}

bool SelectMapScene::OnBackKeyPressed()
{
    SceneManager::GetInstance()->RequestNewScene(new WelcomeScene());
}

void SelectMapScene::OnFramebufferResized(int width, int height)
{
    framebuffer_dimm = glm::vec2(width, height);

    safeAreaCam.onResize(width, height);
    textRenderer_v2->onVievportResize(width, height);
}

void SelectMapScene::OnJoy(float joyX, float joyY)
{

}

void SelectMapScene::OnKeyDown(int keyCode)
{

}

void SelectMapScene::OnPause()
{

}

