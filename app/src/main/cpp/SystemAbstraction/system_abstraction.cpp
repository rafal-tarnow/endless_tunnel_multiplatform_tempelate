//#include "system_abstraction.hpp"
//
//#include "system_log.hpp"
//#include <library_opengles_2/TextRenderer/TextRenderer_v2.hpp>
//#include <SOIL.h>
//#include <sstream>
//#include <iomanip>
//#include <CapAfri/map_editor.hpp>
//
//using namespace  std;
//
//
//
//MapEditor * mapEditor;
//static unsigned int current_width = 640;
//static unsigned int current_height = 480;
//
//
//void SystemAbstraction::onInit(unsigned int width, unsigned int height)
//{
//    current_width = width;
//    current_height = height;
//    mapEditor = new MapEditor(current_width, current_height);
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
//void SystemAbstraction::onResize(unsigned int width, unsigned int height)
//{
//    current_width = width;
//    current_height = height;
//    mapEditor->systemCallback_WindowResize(current_width, current_height);
//}
//
//void SystemAbstraction::onRenderFirstFrame()
//{
//
//}
//
//void SystemAbstraction::onRenderFrame()
//{
//    mapEditor->systemCallback_Render();
//}
//
//void SystemAbstraction::onScroll(double yoffset)
//{
//    mapEditor->systemCallback_Scroll(yoffset);
//}
//
//void SystemAbstraction::onMouseButton(MouseButton mouseButton, ButtonEvent event, int x, int y)
//{
//
//}
//
//void SystemAbstraction::onKeyboard(ButtonEvent event,int key, int x, int y )
//{
//    mapEditor->systemCallback_keyboard(event, key, 0 , 0);
//}
//
//bool SystemAbstraction::onBackKeyPressed()
//{
//    return true;
//}
//
//void SystemAbstraction::onJoyUpdate(float joyX, float joyY)
//{
//
//}
//
//void SystemAbstraction::onPointerDown(int pointerId, const struct PointerCoords *coords)
//{
//
//}
//
//void SystemAbstraction::onPointerUp(int pointerId, const struct PointerCoords *coords)
//{
//
//}
//
//void SystemAbstraction::onPointerMove(int pointerId, const struct PointerCoords *coords)
//{
//    mapEditor->systemCallback_mousePosition(coords->x,coords->y);
//}
//
//void SystemAbstraction::onTimerTick()
//{
//
//}
//
//void SystemAbstraction::onUninit()
//{
//
//}
//



#include "system_abstraction.hpp"
#include "./Application/Tunnel/scene_manager.hpp"
#include "./Application/Tunnel/welcome_scene.hpp"
#include "system_log.hpp"
#include <library_opengles_2/TextRenderer/TextRenderer_v2.hpp>
#include <SOIL.h>
#include <sstream>
#include <iomanip>
#include <CapAfri/game.hpp>

using namespace  std;


static SceneManager *mgr = SceneManager::GetInstance();

#include "./Application/libs/library_opengles_2/RectangleRenderer/Rectangle_Renderer.hpp"
#include "./Application/Tunnel/data/design_graffiti_agentorange_www_myfontfree_com.ttf.hpp"
#include <ft2build.h>
#include FT_FREETYPE_H

#include <bitset>

void drawGlyphToConsole(FT_Face &face){
    for(unsigned int i = 0; i < face->glyph->bitmap.rows; i++){
        for(unsigned int j = 0; j < face->glyph->bitmap.width; j++){

            int alpha_value =   (int)(face->glyph->bitmap.buffer[i*face->glyph->bitmap.width + j]) / 26;

            if(alpha_value > 0){
                LOGD("%d", alpha_value);
            }else{
                LOGD(" ");
            }


        }
        LOGD("\n");
    }
}


TextRenderer_v2 * textRenderer_v2 = nullptr;

void SystemAbstraction::onInit(unsigned int width, unsigned int height)
{
    mgr->StartGraphics(width, height);

    textRenderer_v2 = new TextRenderer_v2(width,height);
    GLuint font_size = float(height)*0.083f;
    textRenderer_v2->LoadFromMemory(design_graffiti_agentorange_www_myfontfree_com_ttf, size_of_design_graffiti_agentorange_www_myfontfree_com_ttf, font_size);

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

    textRenderer_v2->onVievportResize(width, height);
}

void SystemAbstraction::onRenderFirstFrame()
{
    mgr->RequestNewScene(new WelcomeScene());
}

void SystemAbstraction::onRenderFrame()
{
    mgr->DoFrame();

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    //rectangle.model = glm::mat4(1);
    //rectangle.projection = glm::mat4(1);
    //rectangle.view = glm::mat4(1);

    // DE_drawRectangle(&rectangle);

    static int cash = 0;
    stringstream text;
    text << std::fixed << std::setprecision(0) << "$ "<< cash++;


    textRenderer_v2->RenderText(text.str(), 0, 50);

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


