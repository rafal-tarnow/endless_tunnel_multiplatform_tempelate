#include "scene_select_map.hpp"
#include "play_capafri_scene.hpp"
#include "scene_manager.hpp"
#include "scene_tuning_vehicle.hpp"
#include "welcome_scene.hpp"
#include <iomanip>
#include <iostream>
#include <library_opengles_2/Resources/Resources.hpp>
#include <library_opengles_2/Shader/ShadersSources/texture_shader_source.hpp>
#include <library_opengles_2/TextureManager/texture_manager.hpp>
#include <library_opengles_2/TextureManager/texture_manager.hpp>
#include <sstream>
#include <system_billing.hpp>
#include <system_opengl_include.hpp>

using namespace std;

float SelectMapScene::translate = 0.0f;

SelectMapScene::SelectMapScene()
{
    currentMapIndex = cfg->currentMapIndex;

    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
    glDisable(GL_DEPTH_TEST);

    glm::vec2 safe_area_dimension = glm::vec2(1920.0f, 1080.0f);
    safeAreaCam.setSafeAreaDim(safe_area_dimension);

    GLuint txtId = TextureManager::getInstance()->getTextureId("textures/select_map.png");
    DE_initRectangle_3(&safe_area_background, txtId, safe_area_dimension);
    shader = ShaderManager::getInstance()->getShaderFromSource("texture_shader_source.hpp", texture_vertex_shader_source, texture_fragment_shader_source);
    DE_setShader(&safe_area_background, shader);
    DE_setModel(&safe_area_background, glm::translate(glm::mat4(1), glm::vec3(safe_area_dimension.x / 2.0f, safe_area_dimension.y / 2.0f, 0.0f)));

    initNormalButtons();
    initRadioButtons();

    vector<glm::vec3> verticles;
    verticles.push_back(glm::vec3(0, 0, 0));
    verticles.push_back(glm::vec3(1920, 0, 0));
    verticles.push_back(glm::vec3(1920, 1080, 0));
    verticles.push_back(glm::vec3(0, 1080, 0));
    PR_init(&testPrimitive, verticles.data(), 4, glm::vec4(1.0, 0.0, 0.0, 1.0), GL_TRIANGLE_FAN, GL_DYNAMIC_DRAW);

    AudioManager &audioManager = AudioManager::GetSingleton();
    std::string musicName("sounds/music_menu.wav");
    menuMusicHandle = audioManager.CreateSFX(musicName, true);

    translate = 0.0f;
}

void SelectMapScene::initNormalButtons()
{
    glm::vec3 position = glm::vec3(1920.0f * (6.0f / 7.0f), 1080.0f * (1.0f / 6.0f), 0.0f);
    buttonPlay.setPosition(position);
    buttonPlay.setMatrices(&safeAreaCam.viewport(), &safeAreaCam.projection(), &safeAreaCam.view());
    buttonPlay.setNormalBackgroundTexture(TextureManager::getInstance()->getTextureId("textures/continue.png"));
    buttonPlay.setPressedBackgroundTexture(TextureManager::getInstance()->getTextureId("textures/continue_pressed.png"));
    buttonPlay.setEventListener(this);

    position = glm::vec3(1920.0f * (1.0f / 7.0f), 1080.0f * (1.0f / 6.0f), 0.0f);
    buttonUnlock.setPosition(position);
    buttonUnlock.setMatrices(&safeAreaCam.viewport(), &safeAreaCam.projection(), &safeAreaCam.view());
    buttonUnlock.setNormalBackgroundTexture(TextureManager::getInstance()->getTextureId("textures/unlock.png"));
    buttonUnlock.setPressedBackgroundTexture(TextureManager::getInstance()->getTextureId("textures/unlock_pressed.png"));
    buttonUnlock.setEventListener(this);
}

void SelectMapScene::initRadioButtons()
{

    stringstream stream;

    for (unsigned int i = 0; i < LEVELS_COUNT; i++)
    {
        button_1 = new RadioButton();
        stream.str("");
        stream << i + 1;
        button_1->setText(stream.str());
        button_1->setMatrices(&safeAreaCam.viewport(), &safeAreaCam.projection(), &safeAreaCam.view());
        button_1->setDimm(glm::vec2(200, 200));
        button_1->setNormalBackgroundTexture(TextureManager::getInstance()->getTextureId("textures/level_icon.png"));
        button_1->setPressedBackgroundTexture(TextureManager::getInstance()->getTextureId("textures/level_icon_mark.png"));
        button_1->setLockTexture(TextureManager::getInstance()->getTextureId("textures/lock_black.png"));
        if (i > 0)
        {
            button_1->setLockable(true);
            button_1->setLocked(true);
        }

        buttons.push_back(button_1);
    }
    buttons.at(currentMapIndex)->setRadioState(true);

    for (unsigned int i = 0; i < buttons.size(); i++)
    {
        radioButtonManager.addRadioButton(buttons.at(i));
    }

    radioButtonManager.setEventListener(this);



    initMessageBox();

}

void SelectMapScene::initMessageBox()
{
    message_widget = new Widget();
    glm::vec2 dimm = glm::vec2(400,400);
    message_widget->setDimm(dimm);

    label_widget = new Widget();
    dimm = glm::vec2(350,100);
    label_widget->setDimm(dimm);
    label_widget->setModel(glm::translate(glm::mat4(1),glm::vec3(0,100,0)));

    message_widget->addChild(label_widget);

    unlock_widget = new Widget();
    dimm = glm::vec2(100,100);
    unlock_widget->setDimm(dimm);
    unlock_widget->setModel(glm::translate(glm::mat4(1),glm::vec3(120,-50,0)));

    message_widget->addChild(unlock_widget);

    cancel_widget = new Widget();
    dimm = glm::vec2(100,100);
    cancel_widget->setDimm(dimm);
    cancel_widget->setModel(glm::translate(glm::mat4(1),glm::vec3(-120,-50,0)));

    message_widget->addChild(cancel_widget);

}

SelectMapScene::~SelectMapScene()
{

    DE_deleteRectangle(&safe_area_background);

    PR_delete(&testPrimitive);

    for (unsigned int i = 0; i < buttons.size(); i++)
    {
        delete buttons.at(i);
    }

    delete message_widget;
    delete label_widget;
    delete unlock_widget;
    delete cancel_widget;


    cfg->currentMapIndex = currentMapIndex;
    cfg->sync();
}

void SelectMapScene::OnStartGraphics(int width, int height)
{
    AudioManager::GetSingleton().PlaySFX(menuMusicHandle);

    framebuffer_dimm = glm::vec2(width, height);

    safeAreaCam.onResize(width, height);

    GLuint fontSize = GLuint(float(height) * 0.076f);
    //    Resource font_design_graffiti_agentorange("fonts/design_graffiti_agentorange_www_myfontfree_com.ttf");
    // Resource font_design_graffiti_agentorange("fonts/arial.ttf");

    //    textRenderer_v2 = new TextRenderer_v2(width,height, glm::vec4(1,0,0,1));
    //    textRenderer_v2->LoadFromMemory("Design graffiti agentorange", font_design_graffiti_agentorange.getData(), font_design_graffiti_agentorange.getSize(), fontSize);
}

void SelectMapScene::OnKillGraphics()
{
    AudioManager::GetSingleton().StopSFX(menuMusicHandle);
}

void SelectMapScene::DoFrame()
{
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
    glDisable(GL_DEPTH_TEST);

    // glEnable(GL_STENCIL_TEST);
    glStencilFunc(GL_ALWAYS, 1, 0xFF);
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    static float x_pos = 0;
    x_pos += 0.5f;

    glm::mat4 all_model = glm::translate(glm::mat4(1), glm::vec3(0, 700, 0));

    testPrimitive.model = glm::mat4(1);

    glStencilMask(0xFF);
    glClear(GL_STENCIL_BUFFER_BIT);

    PR_setColour(&testPrimitive, glm::vec4(0.7, 0.7, 0.7, 1.0));

    vector<glm::vec3> verticles;
    verticles.push_back(glm::vec3(0, 0, 0));
    verticles.push_back(glm::vec3(1920, 0, 0));
    verticles.push_back(glm::vec3(1920, 1080, 0));
    verticles.push_back(glm::vec3(0, 1080, 0));
    PR_setVerticles(&testPrimitive, verticles.data(), 4);

    // PR_draw(&testPrimitive, 1.0);

    glStencilMask(0x00);
    glStencilFunc(GL_EQUAL, 1, 0xFF);

    verticles.clear();
    verticles.push_back(glm::vec3(-200, 200, 0));
    verticles.push_back(glm::vec3(-200, -200, 0));
    verticles.push_back(glm::vec3(200, -200, 0));
    verticles.push_back(glm::vec3(200, 200, 0));
    PR_setVerticles(&testPrimitive, verticles.data(), 4);
    PR_setColour(&testPrimitive, glm::vec4(1.0, 0.0, 0.0, 1.0));

    glm::mat4 model_h = glm::mat4(1);
    glm::mat4 model_v = glm::mat4(1);
    glm::vec3 horizontal_translation = glm::vec3(220, 0, 0);
    glm::vec3 vertical_translation = glm::vec3(0, -220, 0);

    unsigned int column_index = 0;

    for (unsigned int i = 0; i < buttons.size(); i++)
    {
        model_h = glm::translate(model_h, horizontal_translation);
        buttons.at(i)->setModel(all_model * model_h * model_v);

        column_index++;
        if (column_index == 8)
        {
            column_index = 0;
            model_v = glm::translate(model_v, vertical_translation);
            model_h = glm::mat4(1);
        }
    }



    for (unsigned int i = 0; i < buttons.size(); i++)
    {
        buttons.at(i)->Render();
    }


    translate += 0.5;
    static float scale = 1.0;
    static bool rise = true;
    if(rise)
    {
        scale += 0.1;
        if(scale > 2.5)
            rise = false;
    }
    else
    {
        scale -= 0.1;
        if(scale < 1.0)
            rise = true;
    }

    glm::mat4 M = glm::scale(glm::translate(glm::mat4(1), glm::vec3(translate, 1080 / 2, 0)),glm::vec3(1.0,1.0,1.0));
    //glm::mat4 M = glm::translate(glm::mat4(1), glm::vec3(1920 / 2, 1080 / 2, 0));

    message_widget->Render(safeAreaCam.viewport(), safeAreaCam.projection(), safeAreaCam.view(), M);


    safe_area_background.projection = safeAreaCam.projection();
    safe_area_background.view = safeAreaCam.view();

    DE_drawRectangle(&safe_area_background);

    buttonPlay.Render();
    buttonUnlock.Render();

    testPrimitive.projection = safeAreaCam.projection();
    testPrimitive.view = safeAreaCam.view();

    // glDisable(GL_STENCIL_TEST);
}

void SelectMapScene::Button_onClicked(Button *button)
{
    if (button == &buttonPlay)
    {
        cfg->currentMapIndex = currentMapIndex;

        SceneManager *mgr = SceneManager::GetInstance();
        mgr->RequestNewScene(new TuningVehicleScene());
    }
    else if (button == &buttonUnlock)
    {
        purchase("Purchase product!");
    }
}

void SelectMapScene::RadioButtonManager_onRadioButtonChanged(RadioButton *radioButton)
{
    for (unsigned int i = 0; i < buttons.size(); i++)
    {
        if (buttons.at(i) == radioButton)
        {
            currentMapIndex = i;
            break;
        }
    }

    LOGD("Button %s was clicked and currentMapIndex = %d \n", radioButton->getText().c_str(), currentMapIndex);
}

void SelectMapScene::OnPointerDown(int pointerId, const struct PointerCoords *coords)
{
    for (unsigned int i = 0; i < buttons.size(); i++)
    {
        buttons.at(i)->onPointerDown(coords->x, coords->y);
    }

    buttonPlay.onPointerDown(coords->x, coords->y);
    buttonUnlock.onPointerDown(coords->x, coords->y);
    message_widget->onPointerDown(coords->x, coords->y);
}

void SelectMapScene::OnPointerUp(int pointerId, const struct PointerCoords *coords)
{
    for (unsigned int i = 0; i < buttons.size(); i++)
    {
        buttons.at(i)->onPointerUp();
    }

    buttonPlay.onPointerUp();
    buttonUnlock.onPointerUp();
    message_widget->onPointerUp();
}

void SelectMapScene::OnPointerMove(int pointerId, const struct PointerCoords *coords)
{

}

bool SelectMapScene::OnBackKeyPressed()
{
    SceneManager::GetInstance()->RequestNewScene(new WelcomeScene());
    return true;
}

void SelectMapScene::OnFramebufferResized(int width, int height)
{
    framebuffer_dimm = glm::vec2(width, height);

    safeAreaCam.onResize(width, height);
    //    textRenderer_v2->onVievportResize(width, height);
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
