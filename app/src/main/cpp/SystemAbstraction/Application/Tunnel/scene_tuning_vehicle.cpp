#include "scene_tuning_vehicle.hpp"
#include <system_opengl_include.hpp>
#include "scene_manager.hpp"
#include "play_capafri_scene.hpp"

TuningVehicleScene::TuningVehicleScene()
{
    vehicleTuningGui_setEventListener(this);
    set_style(demo_getContext(),THEME_BLACK);
}

TuningVehicleScene::~TuningVehicleScene()
{
    vehicleTuningGui_setEventListener(nullptr);
}


void TuningVehicleScene::OnStartGraphics(int width, int height)
{
    framebuffer_width = width;
    framebuffet_height = height;
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
    //glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
    glClearColor(0.0f, 1.0f, 0.0f, 1.0f);



    vehicleTuningGui_render(framebuffer_width, framebuffet_height);

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

}

void TuningVehicleScene::OnFramebufferResized(int width, int height)
{
    framebuffer_width = width;
    framebuffet_height = height;
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
