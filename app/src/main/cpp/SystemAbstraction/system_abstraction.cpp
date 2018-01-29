
#include "system_abstraction.hpp"
#include "./Application/scene_manager.hpp"
#include "./Application/welcome_scene.hpp"
#include <SOIL.h>

static SceneManager *mgr = SceneManager::GetInstance();

void SystemAbstraction::onInit(unsigned int width, unsigned int height)
{
    mgr->StartGraphics();
}

void SystemAbstraction::onPause()
{
    mgr->OnPause();
}

void SystemAbstraction::onResume()
{
    mgr->OnResume();
}

void SystemAbstraction::onResize(unsigned int width, unsigned int height)
{
    mgr->SetScreenSize(width, height);
    glViewport(0, 0, width, height);
}

void SystemAbstraction::onRenderFirstFrame()
{
    mgr->RequestNewScene(new WelcomeScene());
}

void SystemAbstraction::onRenderFrame()
{
    mgr->DoFrame();
}

void SystemAbstraction::onScroll(double yoffset)
{

}

void SystemAbstraction::onMouseButton(MouseButton mouseButton, ButtonEvent event, int x, int y)
{

}

void SystemAbstraction::onKeyboard(ButtonEvent event,int key, int x, int y )
{
    if(event == EVENT_DOWN) {
       // mgr->OnKeyDown(key);
    }else if(event == EVENT_UP){
       // mgr->OnKeyUp(key);
    }
}

bool SystemAbstraction::onBackKeyPressed()
{
    return mgr->OnBackKeyPressed();
}

void SystemAbstraction::onJoyUpdate(float joyX, float joyY)
{
    mgr->UpdateJoy(joyX, joyY);
}

void SystemAbstraction::onPointerDown(int pointerId, const struct PointerCoords *coords)
{
    mgr->OnPointerDown(pointerId, coords);
}

void SystemAbstraction::onPointerUp(int pointerId, const struct PointerCoords *coords)
{
    mgr->OnPointerUp(pointerId, coords);
}

void SystemAbstraction::onPointerMove(int pointerId, const struct PointerCoords *coords)
{
    mgr->OnPointerMove(pointerId, coords);
}

void SystemAbstraction::onTimerTick()
{

}

void SystemAbstraction::onUninit()
{
    mgr->KillGraphics();
}
