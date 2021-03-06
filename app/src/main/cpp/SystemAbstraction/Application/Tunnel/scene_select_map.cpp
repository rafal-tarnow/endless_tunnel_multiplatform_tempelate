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
#include <system_opengl_include.hpp>
#include "../CapAfri/products.h"

using namespace std;

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

    //List current products
    products = Billing::listOwnedProducts();
    string txt;
    for(auto it = products.begin(); it != products.end(); it++)
    {
        txt.append("|");
        txt.append(*it);
        txt.append("|");
    }
    printToast("Billing::listOwnedProducts(): " + txt);

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

    Billing::setEventListener(this);
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
        button = new RadioButton();
        stream.str("");
        stream << i + 1;
        button->setText(stream.str());
        button->setMatrices(&safeAreaCam.viewport(), &safeAreaCam.projection(), &safeAreaCam.view());
        button->setDimm(glm::vec2(200, 200));
        button->setNormalBackgroundTexture(TextureManager::getInstance()->getTextureId("textures/level_icon.png"));
        button->setPressedBackgroundTexture(TextureManager::getInstance()->getTextureId("textures/level_icon_mark.png"));
        button->setLockTexture(TextureManager::getInstance()->getTextureId("textures/lock_black.png"));

        if (i == 1)
        {
            if(products.count(PRODUCT_UNLOCK_LEVEL_2) == 0) {
                button->setLockable(true);
                button->setLocked(true);
            }
        }

        if (i == 2)
        {
            if(products.count(PRODUCT_UNLOCK_LEVEL_3) == 0) {
                button->setLockable(true);
                button->setLocked(true);
            }
        }


        buttons.push_back(button);
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
    glm::vec2 dimm = glm::vec2(800,800);
    message_widget->setDimm(dimm);
    message_widget->setNormalBackgroundTexture(TextureManager::getInstance()->getTextureId("textures/message_box.png"));
    message_widget->setPressedBackgroundTexture(TextureManager::getInstance()->getTextureId("textures/message_box.png"));

    dimm = glm::vec2(600,80);
    label_widget = new Label(72);
    label_widget->setDimm(dimm);
    label_widget->setModel(glm::translate(glm::mat4(1),glm::vec3(0,270,0)));

    message_widget->addChild(label_widget);

    explain_widget = new Explain();
    dimm = glm::vec2(700,100);
    explain_widget->setDimm(dimm);
    explain_widget->setModel(glm::translate(glm::mat4(1),glm::vec3(0,80,0)));

    message_widget->addChild(explain_widget);

    unlock_widget = new Widget();
    dimm = glm::vec2(200,200);
    unlock_widget->setDimm(dimm);
    unlock_widget->setModel(glm::translate(glm::mat4(1),glm::vec3(160,-200,0)));
    unlock_widget->setNormalBackgroundTexture(TextureManager::getInstance()->getTextureId("textures/continue_white.png"));
    unlock_widget->setPressedBackgroundTexture(TextureManager::getInstance()->getTextureId("textures/continue_white.png"));
    unlock_widget->setEventListener(this);

    message_widget->addChild(unlock_widget);

    cancel_widget = new Widget();
    dimm = glm::vec2(200,200);
    cancel_widget->setDimm(dimm);
    cancel_widget->setModel(glm::translate(glm::mat4(1),glm::vec3(-160,-200,0)));
    cancel_widget->setNormalBackgroundTexture(TextureManager::getInstance()->getTextureId("textures/replay_white.png"));
    cancel_widget->setPressedBackgroundTexture(TextureManager::getInstance()->getTextureId("textures/replay_white.png"));
    cancel_widget->setEventListener(this);

    message_widget->addChild(cancel_widget);

}

SelectMapScene::~SelectMapScene()
{
    Billing::setEventListener(nullptr);

    DE_deleteRectangle(&safe_area_background);

    PR_delete(&testPrimitive);

    for (unsigned int i = 0; i < buttons.size(); i++)
    {
        delete buttons.at(i);
    }

    delete message_widget;
    delete label_widget;
    delete explain_widget;
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

    //GLuint fontSize = GLuint(float(height) * 0.076f);
    // Resource font_design_graffiti_agentorange("fonts/design_graffiti_agentorange_www_myfontfree_com.ttf");
    // Resource font_design_graffiti_agentorange("fonts/arial.ttf");

    //    textRenderer_v2 = new TextRenderer_v2(width,height, glm::vec4(1,0,0,1));
    //    textRenderer_v2->LoadFromMemory("Design graffiti agentorange", font_design_graffiti_agentorange.getData(), font_design_graffiti_agentorange.getSize(), fontSize);


    mEffects = new PostProcessor(width, height);
}

void SelectMapScene::OnKillGraphics()
{
    AudioManager::GetSingleton().StopSFX(menuMusicHandle);

    if (mEffects)
    {
        delete mEffects;
        mEffects = nullptr;
    }
}

void SelectMapScene::DoFrame()
{
    static GLfloat time = 0.0f;
    time += 0.017f;

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

    //if(isMessageBoxVisible)
        mEffects->Background = GL_TRUE;
    //else
        //mEffects->Background = GL_FALSE;

    mEffects->opaque = opaque;

    mEffects->BeginRender(glm::vec4(1.0,1.0,1.0,1.0));
    {


        for (unsigned int i = 0; i < buttons.size(); i++)
        {
            buttons.at(i)->Render();
        }


        safe_area_background.projection = safeAreaCam.projection();
        safe_area_background.view = safeAreaCam.view();

        DE_drawRectangle(&safe_area_background);

        buttonPlay.Render();
        buttonUnlock.Render();




        testPrimitive.projection = safeAreaCam.projection();
        testPrimitive.view = safeAreaCam.view();

    }
    mEffects->EndRender();
    mEffects->Render(time);


    static float alfa = 0.0f;
    alfa += 0.01;


    if(isMessageBoxVisible)
    {
        glm::mat4 M = glm::rotate((glm::translate(glm::mat4(1), glm::vec3(position_x /*+ 10*glm::sin(15*time)*/, position_y /*+ 10*glm::cos(15*time + 2)*/, 0))),0.0f/*0.05f*sin(20*alfa)*/, glm::vec3(0,0,1.0));
        message_widget->setMatrices(safeAreaCam.viewport(), safeAreaCam.projection(), safeAreaCam.view());
        message_widget->setModel(M);
        message_widget->Render();
    }
    // glDisable(GL_STENCIL_TEST);

    position_x.update(time);
    position_y.update(time);
    opaque.update(time);
}

void SelectMapScene::Button_onClicked(Button *button)
{
    if (button == &buttonPlay)
    {
        cfg->currentMapIndex = currentMapIndex;

        if(buttons.at(currentMapIndex)->isLocked())
        {
            isMessageBoxVisible = true;
            position_x = 1920.0f/2.0f;
            position_y = 1080.0f/2.0f;
            opaque = 0.5;
        }
        else
        {
            SceneManager *mgr = SceneManager::GetInstance();
            mgr->RequestNewScene(new TuningVehicleScene());
        }
    }
    else if (button == &buttonUnlock)
    {
        Billing::purchaseProduct("android.test.purchased");
    }
}

void SelectMapScene::Widget_onClicked(Widget * widget)
{
    if(widget == cancel_widget)
    {
        isMessageBoxVisible = false;
        position_x = 0;
        position_y = 0;
        opaque = 1.0f;
    }
    else if(widget == unlock_widget)
    {
        Billing::purchaseProduct("android.test.purchased");
        isMessageBoxVisible = false;
        position_x = 0;
        position_y = 0;
        opaque = 1.0f;
    }

}

void SelectMapScene::Billing_onOwnedProductsListChanged(set<string> products)
{
    for (unsigned int i = 0; i < LEVELS_COUNT; i++)
    {
        if (i == 1)
        {
            if(products.count(PRODUCT_UNLOCK_LEVEL_2) > 0) {
                buttons.at(i)->setLocked(false);
            }
        }

        if (i == 2)
        {
            if(products.count(PRODUCT_UNLOCK_LEVEL_3) > 0) {
                buttons.at(i)->setLocked(false);
            }
        }
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
    if(!isMessageBoxVisible)
    {
        for (unsigned int i = 0; i < buttons.size(); i++)
        {
            buttons.at(i)->onPointerDown(coords->x, coords->y);
        }

        buttonPlay.onPointerDown(coords->x, coords->y);
        buttonUnlock.onPointerDown(coords->x, coords->y);
    }
    else
    {
        message_widget->onPointerDown(coords->x, coords->y);
    }
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
    if(isMessageBoxVisible)
    {
        isMessageBoxVisible = false;
        position_x = 0;
        position_y = 0;
        opaque = 1.0f;
    }
    else
    {
         SceneManager::GetInstance()->RequestNewScene(new WelcomeScene());
    }

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
