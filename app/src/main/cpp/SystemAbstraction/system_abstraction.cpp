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
#include <CapAfri/game_config.hpp>

#include <library_opengles_2/Debug/Debug.hpp>

using namespace  std;

static FPS fps;
static DeltaTimer deltaTimer;
static SceneManager *mgr = SceneManager::GetInstance();
static TextRenderer_v2 * textRenderer_v2;


#include "./Application/libs/library_opengles_2/RectangleRenderer/Rectangle_Renderer.hpp"

#include <bitset>


int SystemAbstraction::framebuffer_width = 0;
int SystemAbstraction::framebuffer_height = 0;


class Srednia
{
    Srednia(int size)
    {

    }

private:
};

  stringstream text;


GameConfig *cfg;

void SystemAbstraction::onInit(unsigned int fb_width, unsigned int fb_height)
{
    framebuffer_width = fb_width;
    framebuffer_height = fb_height;
    mgr->StartGraphics(framebuffer_width, framebuffer_height);


    float scale = framebuffer_height/1080.0f;
    demo_init(framebuffer_width, framebuffer_height, scale);

    GLuint fontSize = GLuint(float(fb_height)*0.020f);
    textRenderer_v2 = new TextRenderer_v2(fb_width,fb_height, glm::vec4(1.0, 1.0, 1.0, 1.0));
    Resource font_arial("fonts/arial.ttf");
    textRenderer_v2->LoadFromMemory("arial.ttf", font_arial.getData(), font_arial.getSize(), fontSize);

   text << std::fixed << std::setprecision(1);

   cfg = GameConfig::create(getStandardCommonReadWriteDirecory() + "/CapitanAfrica.config");
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

Average<double> averageFPS(600);
Average<double> averageFrameTime(600);

double current_fps = 30;
double average_fps = 30;


void SystemAbstraction::onRenderFrame()
{

        deltaTimer.start();


    mgr->DoFrame();

    if(cfg->waitForGlFinish) {
        glFlush();
        glFinish();
    }

    static double render_time_ms;

    static double render_time_ms_copy;
    static double fpsd_copy;




        render_time_ms = deltaTimer.getTimeFromStart_ms();
        current_fps = fps.getFPS();

        averageFPS.putValue(current_fps);
        average_fps = averageFPS.getSrednia();
        averageFrameTime.putValue(render_time_ms);

        static int skip = 0;
        skip++;

        if (!(skip % 20)) {
            render_time_ms_copy = render_time_ms;
            fpsd_copy = current_fps;
        }

    if(cfg->showFPS) {

        text.str("");
        text << "FPS " << fpsd_copy;

        glDisable(GL_DEPTH_TEST);
        textRenderer_v2->RenderText(1, text.str(), framebuffer_width * 0.95,
                                    framebuffer_height * 0.95, TextRenderer_v2::TEXT_LEFT);

        text.str("");
        text << "AFPS: " << average_fps;

        textRenderer_v2->RenderText(2, text.str(), framebuffer_width * 0.95,
                                    framebuffer_height * 0.93, TextRenderer_v2::TEXT_LEFT);

        text.str("");
        text << "T: " << render_time_ms_copy << "[ms]";

        textRenderer_v2->RenderText(3, text.str(), framebuffer_width * 0.95,
                                    framebuffer_height * 0.91, TextRenderer_v2::TEXT_LEFT);

        text.str("");
        text << "AT: " << averageFrameTime.getSrednia() << "[ms]";

        textRenderer_v2->RenderText(4, text.str(), framebuffer_width * 0.95,
                                    framebuffer_height * 0.89, TextRenderer_v2::TEXT_LEFT);
    }
    ObjectCounter::print();
    cout << flush;

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

    GameConfig::deleteInstance();
}




