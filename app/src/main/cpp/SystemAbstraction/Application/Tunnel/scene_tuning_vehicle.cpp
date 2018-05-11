#include "scene_tuning_vehicle.hpp"
#include <system_opengl_include.hpp>
#include "scene_manager.hpp"
#include "play_capafri_scene.hpp"
#include <SOIL.h>
#include <library_opengles_2/Resources/Resources.hpp>
#include "welcome_scene.hpp"

TuningVehicleScene::TuningVehicleScene()
{
    vehicleTuningGui_setEventListener(this);
    set_style(demo_getContext(),THEME_BLACK);

    glClearColor(0.0f, 1.0f, 0.0f, 1.0f);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
    glDisable(GL_DEPTH_TEST);


    safe_area_dim = glm::vec2(16.0f/9.0f,1.0f);
    DE_initRectangle(&safe_area, "textures/garage.png", glm::vec3(0.0f,0.0f,0.0f),safe_area_dim);

}

TuningVehicleScene::~TuningVehicleScene()
{
    vehicleTuningGui_setEventListener(nullptr);

    DE_deleteRectangle(&safe_area);
    glDeleteTextures(1, &garageTextureId);
}


void TuningVehicleScene::OnStartGraphics(int width, int height)
{
    framebuffer_width = width;
    framebuffer_height = height;
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


    auto framebuffer_aspect = float(framebuffer_width)/float(framebuffer_height);
    auto safe_area_aspect = safe_area_dim.x/safe_area_dim.y;

    glm::mat4 P_GUI;

    if(safe_area_aspect > framebuffer_aspect)
    {
        auto half_width = safe_area_dim.x/2.0f;
        auto half_height = half_width/framebuffer_aspect;
        P_GUI = glm::ortho(-half_width, half_width, -half_height, half_height, -1000.0f, 1000.0f);
    }
    else
    {
        auto half_height = safe_area_dim.y/2.0f;
        auto half_width = half_height*framebuffer_aspect;
        P_GUI = glm::ortho(-half_width, half_width, -half_height, half_height, -1000.0f, 1000.0f);
    }
    //    glm::mat4 V_GUI = glm::lookAt(glm::vec3(float(framebuffer_width)/2.0f, float(framebuffet_height)/2.0f, 1.0f),glm::vec3(float(framebuffer_width)/2.0f, float(framebuffet_height)/2.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    //    glm::mat4 M_GUI = glm::translate(glm::mat4(1),glm::vec3(1920/2, 0,0));

    //glm::mat4 P_GUI = glm::mat4(1);
    glm::mat4 V_GUI = glm::mat4(1);
    glm::mat4 M_GUI = glm::mat4(1);


    safe_area.projection = P_GUI;
    safe_area.view = V_GUI;
    safe_area.model = M_GUI;

    DE_drawRectangle(&safe_area);

    damperButton.Render(P_GUI, V_GUI, M_GUI);

    glFlush();
}

void TuningVehicleScene::OnPointerDown(int pointerId, const struct PointerCoords *coords)
{
    demo_onMouseButtonCallback(SystemAbstraction::MOUSE_LEFT_BUTTON,
                               SystemAbstraction::EVENT_DOWN, (int) coords->x, (int) coords->y);
    if(vehicleTuningGui_isAnyWindowHovered()) //if input is on window, end process events
        return;
}

void TuningVehicleScene::OnPointerUp(int pointerId, const struct PointerCoords *coords)
{
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
    framebuffer_width = width;
    framebuffer_height = height;
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
