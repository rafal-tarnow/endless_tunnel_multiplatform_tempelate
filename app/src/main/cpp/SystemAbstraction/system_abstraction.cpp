#include "system_abstraction.hpp"
#include "./Application/Tunnel/scene_manager.hpp"
#include "./Application/Tunnel/welcome_scene.hpp"
#include "./Application/Tunnel/scene_tuning_vehicle.hpp"
#include "system_log.hpp"
#include <library_opengles_2/TextRenderer/TextRenderer_v2.hpp>
#include <sstream>
#include <iomanip>
#include <CapAfri/game.hpp>
#include <CapAfri/demo.hpp>

using namespace  std;


static SceneManager *mgr = SceneManager::GetInstance();

#include "./Application/libs/library_opengles_2/RectangleRenderer/Rectangle_Renderer.hpp"
#include "./Application/Tunnel/data/design_graffiti_agentorange_www_myfontfree_com.ttf.hpp"

#include <bitset>


int SystemAbstraction::framebuffer_width = 0;
int SystemAbstraction::framebuffer_height = 0;



void SystemAbstraction::onInit(unsigned int fb_width, unsigned int fb_height)
{
    framebuffer_width = fb_width;
    framebuffer_height = fb_height;
    mgr->StartGraphics(framebuffer_width, framebuffer_height);


    float scale = framebuffer_height/1080.0f;
    demo_init(framebuffer_width, framebuffer_height, scale);
}

void SystemAbstraction::onPause()
{
    mgr->OnPause();
}

void SystemAbstraction::onResume()
{
    mgr->OnResume();
}

void SystemAbstraction::onFramebufferResize(unsigned int fb_width, unsigned int fb_height)
{
    framebuffer_width = fb_width;
    framebuffer_height = fb_height;
    mgr->SetScreenSize(framebuffer_width, framebuffer_height);
    glViewport(0, 0, framebuffer_width, framebuffer_height);


    float scale = framebuffer_height/1080.0f;
    demo_setScale(framebuffer_width, framebuffer_height, scale);
}

void SystemAbstraction::onRenderFirstFrame()
{
    mgr->RequestNewScene(new WelcomeScene());
//    mgr->RequestNewScene(new TuningVehicleScene());
}

void SystemAbstraction::onRenderFrame()
{
    mgr->DoFrame();
}

void SystemAbstraction::onMouseScroll(double yoffset)
{
    mgr->OnMouseScroll(yoffset);
}

void SystemAbstraction::onMouseButton(MouseButton mouseButton, ButtonEvent event, int x, int y)
{
    mgr->OnMouseButton(mouseButton, event, x, y);
}

void SystemAbstraction::onMouseMove(int x, int y)
{
    mgr->OnMouseMove(x, y);
}

void SystemAbstraction::onKey(ButtonEvent event, Key key, Mods mods, int x, int y)
{
    mgr->OnKey(event, key, mods, x, y);
}

void SystemAbstraction::onChar(unsigned int codepoint)
{
    mgr->OnChar(codepoint);
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
    //LOGD("--> SystemAbstraction::onPointerDown()\n pointerId = %d", pointerId);
    mgr->OnPointerDown(pointerId, coords);
    //LOGD("<-- SystemAbstraction::onPointerDown()\n");
}

void SystemAbstraction::onPointerUp(int pointerId, const struct PointerCoords *coords)
{
    //LOGD("--> SystemAbstraction::onPointerUp()\n");
    mgr->OnPointerUp(pointerId, coords);
    //LOGD("<-- SystemAbstraction::onPointerUp()\n");
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
    demo_uninit();
}


//#include "system_abstraction.hpp"
//#include "system_log.hpp"
//#include "Application/CapAfri/demo.hpp"
//
//int SystemAbstraction::framebuffer_width = 0;
//int SystemAbstraction::framebuffer_height = 0;
//
//void SystemAbstraction::onInit(unsigned int fb_width, unsigned int fb_height)
//{
//    framebuffer_width = fb_width;
//    framebuffer_height = fb_height;
//    demo_init(framebuffer_width, framebuffer_height, nullptr);
//}
//
//void SystemAbstraction::onPause()
//{
//
//}
//
//void SystemAbstraction::onResume()
//{
//
//}
//
//void SystemAbstraction::onFramebufferResize(unsigned int fb_width, unsigned int fb_height)
//{
//    framebuffer_width = fb_width;
//    framebuffer_height = fb_height;
//}
//
//void SystemAbstraction::onRenderFirstFrame()
//{
//
//}
//
//void SystemAbstraction::onRenderFrame()
//{
//    glClear(GL_COLOR_BUFFER_BIT);
//    glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
//    mapEditorGui_render(framebuffer_width,framebuffer_height);
//}
//
//void SystemAbstraction::onScroll(double yoffset)
//{
//    demo_onScrollCallback(yoffset);
//}
//
//void SystemAbstraction::onMouseButton(MouseButton mouseButton, ButtonEvent event, int x, int y)
//{
//    demo_onMouseButtonCallback(mouseButton, event, x, y);
//}
//
//void SystemAbstraction::onKey(ButtonEvent event, Key key, Mods mods, int x, int y )
//{
//    demo_onKeyCallback(event, key, mods, x, y);
//}
//
//void SystemAbstraction::onChar(unsigned int codepoint)
//{
//    demo_onCharCallback(codepoint);
//}
//
//bool SystemAbstraction::onBackKeyPressed()
//{
//
//}
//
//void SystemAbstraction::onJoyUpdate(float joyX, float joyY)
//{
//
//}
//
//void SystemAbstraction::onPointerDown(int pointerId, const struct PointerCoords *coords)
//{
//    demo_onMouseButtonCallback(MOUSE_LEFT_BUTTON, EVENT_DOWN, (int)coords->x, (int)coords->y);
//}
//
//void SystemAbstraction::onPointerUp(int pointerId, const struct PointerCoords *coords)
//{
//    demo_onMouseButtonCallback(MOUSE_LEFT_BUTTON, EVENT_UP, (int)coords->x, (int)coords->y);
//}
//
//void SystemAbstraction::onPointerMove(int pointerId, const struct PointerCoords *coords)
//{
//    demo_onPointerMoveCallback(pointerId, coords);
//}
//
//void SystemAbstraction::onTimerTick()
//{
//
//}
//
//void SystemAbstraction::onUninit()
//{
//    demo_uninit();
//}


