#include "system_abstraction.hpp"
#include "./Application/Tunnel/scene_manager.hpp"
#include "./Application/Tunnel/welcome_scene.hpp"
#include "./Application/Tunnel/scene_tuning_vehicle.hpp"
#include "system_log.hpp"
#include <library_opengles_2/TextRenderer/TextRenderer_v2.hpp>
#include <library_opengles_2/Resources/Resources.hpp>
#include <sstream>
#include <iomanip>
#include <CapAfri/game.hpp>
#include <CapAfri/demo.hpp>
#include <fps.hpp>

using namespace  std;

static FPS fps;
static DeltaTimer deltaTimer;
static SceneManager *mgr = SceneManager::GetInstance();
static TextRenderer_v2 * textRenderer_v2;


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

    GLuint fontSize = GLuint(float(fb_height)*0.076f);
    textRenderer_v2 = new TextRenderer_v2(fb_width,fb_height, glm::vec4(1.0, 1.0, 1.0, 1.0));
    Resource font_arial("fonts/arial.ttf");
    textRenderer_v2->LoadFromMemory("Arial", font_arial.getData(), font_arial.getSize(), fontSize);
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

     textRenderer_v2->onVievportResize(framebuffer_width, framebuffer_height);
}

void SystemAbstraction::onRenderFirstFrame()
{
    mgr->RequestNewScene(new WelcomeScene());
}

void SystemAbstraction::onRenderFrame()
{
//    deltaTimer.start();
    mgr->DoFrame();

//    static double render_time_ms;
//    static double fpsd;
//    static double render_time_ms_copy;
//    static double fpsd_copy;

//    render_time_ms = deltaTimer.getTimeFromStart_ms();
//    fpsd = fps.getFPS();

//static int skip = 0;
//    skip++;

//    if(!(skip % 20))
//    {
//        render_time_ms_copy = render_time_ms;
//        fpsd_copy = fpsd;
//    }

//    stringstream text;

//    text << std::fixed << std::setprecision(1) << "FPS "<< fpsd_copy;

//    textRenderer_v2->RenderText(text.str(), framebuffer_width*0.75, framebuffer_height*0.9);

//    text.str("");
//    text << "Rnd t: " << render_time_ms_copy <<"[ms]";
//    textRenderer_v2->RenderText(text.str(), framebuffer_width*0.55, framebuffer_height*0.8);

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
      delete textRenderer_v2;

    mgr->KillGraphics();
    demo_uninit();
}




