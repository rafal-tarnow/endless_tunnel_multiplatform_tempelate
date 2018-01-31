
#include "system_abstraction.hpp"
#include "./Application/scene_manager.hpp"
#include "./Application/welcome_scene.hpp"
#include "system_log.hpp"
#include <SOIL.h>

static SceneManager *mgr = SceneManager::GetInstance();


#include "./Application/data/design_graffiti_agentorange_www_myfontfree_com.ttf.hpp"
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

void SystemAbstraction::onInit(unsigned int width, unsigned int height)
{

    FT_Library ft;

    if (FT_Init_FreeType(&ft))
        LOGD("ERROR::FREETYPE: Could not init FreeType Library\n");

    FT_Face face;
    if (FT_New_Memory_Face(ft,design_graffiti_agentorange_www_myfontfree_com_ttf, size_of_design_graffiti_agentorange_www_myfontfree_com_ttf , 0, &face))
        LOGD("ERROR::FREETYPE: Failed to load font\n");

    FT_Set_Pixel_Sizes(face, 0, 32);

    if (FT_Load_Char(face, 'W', FT_LOAD_RENDER /*| FT_LOAD_TARGET_MONO*/))
        LOGD("ERROR::FREETYTPE: Failed to load Glyph\n");

    LOGD("width = %ld\n", face->glyph->metrics.width);
    LOGD("height = %ld\n", face->glyph->metrics.height);

    LOGD("bitmap.width = %d\n", face->glyph->bitmap.width);
    LOGD("bitmap.rows = %d\n", face->glyph->bitmap.rows);
    LOGD("bitmap.pitch = %d\n", face->glyph->bitmap.pitch);

    drawGlyphToConsole(face);

    FT_Done_FreeType(ft);

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
