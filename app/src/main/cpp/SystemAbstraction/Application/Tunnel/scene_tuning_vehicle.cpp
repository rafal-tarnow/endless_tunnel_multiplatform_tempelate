#include "scene_tuning_vehicle.hpp"
#include <system_opengl_include.hpp>
#include "scene_manager.hpp"
#include "play_capafri_scene.hpp"
#include <SOIL.h>
#include <library_opengles_2/Resources/Resources.hpp>
#include <library_opengles_2/TextureManager/texture_manager.hpp>
#include "welcome_scene.hpp"

TuningVehicleScene::TuningVehicleScene()
{
    vehicleTuningGui_setEventListener(this);


    set_style(demo_getContext(),THEME_BLACK);

    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
    glDisable(GL_DEPTH_TEST);


    safe_area_dim = glm::vec2(1920.0f,1080.0f);
    DE_initRectangle(&safe_area, "textures/garage.png", glm::vec3(safe_area_dim.x/2.0f, safe_area_dim.y/2.0f, 0.0f),safe_area_dim);

    DE_initRectangle(&backgroundRect, "textures/bg.png", glm::vec3(0.0f,0.0f,0.0f),safe_area_dim);



    initButtons();


}

void TuningVehicleScene::initButtons()
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

    radioButtonManager.addRadioButton(&button_shockAbsorber);
    radioButtonManager.addRadioButton(&button_spring);
    radioButtonManager.addRadioButton(&button_tires);
}

TuningVehicleScene::~TuningVehicleScene()
{
    vehicleTuningGui_setEventListener(nullptr);

    DE_deleteRectangle(&safe_area);
    DE_deleteRectangle(&backgroundRect);
}


void TuningVehicleScene::OnStartGraphics(int width, int height)
{
    mViewport = glm::vec4(0,0,width,height);
}

void TuningVehicleScene::OnKillGraphics()
{

}

void TuningVehicleScene::gui_onPlayButtonClicked()
{
    SceneManager *mgr = SceneManager::GetInstance();
    mgr->RequestNewScene(new PlayCapAfriScene());
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
    DE_drawRectangle(&backgroundRect);


    safe_area.projection = mProjection;
    safe_area.view = mView;
    safe_area.model = M_GUI;
    DE_drawRectangle(&safe_area);



    button_shockAbsorber.Render();
    button_spring.Render();
    button_tires.Render();

    glFlush();
}

void TuningVehicleScene::OnPointerDown(int pointerId, const struct PointerCoords *coords)
{
    button_shockAbsorber.onPointerDown(coords->x, coords->y);
    button_spring.onPointerDown(coords->x, coords->y);
    button_tires.onPointerDown(coords->x, coords->y);

    demo_onMouseButtonCallback(SystemAbstraction::MOUSE_LEFT_BUTTON,
                               SystemAbstraction::EVENT_DOWN, (int) coords->x, (int) coords->y);
    if(vehicleTuningGui_isAnyWindowHovered()) //if input is on window, end process events
        return;
}

void TuningVehicleScene::OnPointerUp(int pointerId, const struct PointerCoords *coords)
{
    button_shockAbsorber.onPointerUp();
    button_spring.onPointerUp();
    button_tires.onPointerUp();

    demo_onMouseButtonCallback(SystemAbstraction::MOUSE_LEFT_BUTTON,
                               SystemAbstraction::EVENT_UP, (int) coords->x, (int) coords->y);
    if(vehicleTuningGui_isAnyWindowHovered()) //if input is on window, end process events
        return;
}

void TuningVehicleScene::OnPointerMove(int pointerId, const struct PointerCoords *coords)
{
    demo_onPointerMoveCallback(pointerId, coords);
    if(vehicleTuningGui_isAnyWindowHovered()) //if input is on window, end process events
        return;
}

bool TuningVehicleScene::OnBackKeyPressed()
{
    SceneManager::GetInstance()->RequestNewScene(new WelcomeScene());
}

void TuningVehicleScene::OnFramebufferResized(int width, int height)
{
    mViewport = glm::vec4(0,0,width,height);
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

