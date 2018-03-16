#include "map_editor.hpp"
#include <system_opengl_include.hpp>
#include <system_log.hpp>
#include <iostream>
#include <SOIL.h>
#include "./data/red_dot.png.hpp"
#include <system_paths.hpp>
#include <system_log.hpp>
#include "demo.hpp"
#include "design_graffiti_agentorange_www_myfontfree_com.ttf.hpp"
#include "../system_abstraction.hpp"

using namespace std;

MapEditor::MapEditor(int fb_width, int fb_height)
{
    framebuffer_width = fb_width;
    framebuffer_height = fb_height;

    systemCallback_WindowResize(framebuffer_width, framebuffer_height);

    //glEnable(GL_MULTISAMPLE);
    glEnable (GL_BLEND);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glClearColor(0.0,0.0,0.0,1.0);

    redDotTextureId = SOIL_load_OGL_texture_from_memory(red_dot_png, size_of_red_dot_png, 4,0,SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y);
    DE_initRectangle(&redDotPointerRectangle, &redDotTextureId,0.25f, 0.25f, 0.0f);

    gridLines = new CGridLines(0, 1000, 50, 0);


    textRenderer_v2 = new TextRenderer_v2(fb_width, fb_height);
    textRenderer_v2->LoadFromMemory(design_graffiti_agentorange_www_myfontfree_com_ttf, size_of_design_graffiti_agentorange_www_myfontfree_com_ttf, fb_height*0.06);

    //COORDINATES LINES
    float xlineVerticles[6] = {-1000.0f, 0.0f, 0.0f, 1000.0f, 0.0f, 0.0f};
    LS_init(&x_lineStrip,xlineVerticles,6);

    float ylineVerticles[6] = {0.0f, 1000.0f, 0.0f, 0.0f, -1000.0f, 0.0f};
    LS_init(&y_lineStrip,ylineVerticles,6);

    //LEVEL LOAD
    mapFilePath = getAppConfigFilePath() + "/CapitanAfrica.map";
    mapFileOpenErrno = level.loadLevelFromFile(mapFilePath);
    if(mapFileOpenErrno)
    {
        mapFileOpenErrorString = strerror(mapFileOpenErrno);
    }
    LS_init(&lineStripGround, level.ground_verticles.data(), level.ground_verticles.size());


    demo_init(framebuffer_width, framebuffer_height, this);
}

MapEditor::~MapEditor()
{

    delete gridLines;
    DE_deleteRectangle(&redDotPointerRectangle);
    LS_delete(&x_lineStrip);
    LS_delete(&y_lineStrip);
    glDeleteTextures(1,&redDotTextureId);

    demo_uninit();
}

void MapEditor::systemCallback_Render()
{
    glViewport(0,0,framebuffer_width, framebuffer_height);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0.3f, 0.3f, 0.3f, 1.0f);


    if(mapFileOpenErrno != 0)
    {
        textRenderer_v2->RenderText("ERROR WHILE OPEN FILE:",framebuffer_width*0.1,framebuffer_height*0.6);
        textRenderer_v2->RenderText(mapFilePath,framebuffer_width*0.1,framebuffer_height*0.5);
        textRenderer_v2->RenderText(mapFileOpenErrorString,framebuffer_width*0.1,framebuffer_height*0.4);
        return;
    }

    glDisable(GL_DEPTH_TEST);
    {

        glLineWidth(1.0);

        glm::mat4 PVM = camera.getProjectionMatrix()*camera.getViewMatrix()*glm::mat4(1);

        gridLines->Render(glm::value_ptr(PVM), glm::value_ptr(glm::mat4(1)), glm::value_ptr(glm::mat4(1)));

        redDotPointerRectangle.projection = camera.getProjectionMatrix();
        redDotPointerRectangle.view = camera.getViewMatrix();
        redDotPointerRectangle.model = redDotCursorModel;

        DE_drawRectangle(&redDotPointerRectangle);


        for(unsigned int i = 0; i < dots.size(); i++)
        {
            glm::mat4 model = glm::translate(glm::mat4(1),dots[i]);
            redDotPointerRectangle.model = model;
            DE_drawRectangle(&redDotPointerRectangle);
        }

        for (auto & coin : level.coins_vector){
            coin->render(camera.getProjectionMatrix(),camera.getViewMatrix());
        }


        //    viewMatrix = glm::lookAt(glm::vec3(0.0f, 0.0f, 10.0f),glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));


        //    if(car != nullptr)
        //    {
        //        float x, y;
        //        car->getPosition(&x,&y);
        //        viewMatrix = glm::lookAt(glm::vec3(x, y, 10.0f),glm::vec3(x, y, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        //    }

        //    b2Body* tmp=world->GetBodyList();
        //    while(tmp)
        //    {
        //        if(tmp->GetUserData() != nullptr){
        //            ((RenderableObject *)tmp->GetUserData())->render(projectionMatrix, viewMatrix);
        //        }
        //        tmp=tmp->GetNext();
        //    }



        //    static int cash = 0;
        //    stringstream text;
        //    if(car){
        //        text << std::fixed << std::setprecision(0) << "$ "<<car->getXPosition();
        //    }else{
        //        text << "$ 0";
        //    }

        //    textRenderer_v2->RenderText(text.str(), framebuffer_width - 200.0f, framebuffer_height - 50.0f);


        //DRAW COORDINATES LINES
        x_lineStrip.projection = camera.getProjectionMatrix();
        x_lineStrip.view = camera.getViewMatrix();
        x_lineStrip.model = glm::mat4(1);
        LS_draw(&x_lineStrip, 2);

        y_lineStrip.projection = camera.getProjectionMatrix();
        y_lineStrip.view = camera.getViewMatrix();
        y_lineStrip.model = glm::mat4(1);
        LS_draw(&y_lineStrip, 2);

        //DRAW GROUND LINE
        lineStripGround.projection = camera.getProjectionMatrix();
        lineStripGround.view = camera.getViewMatrix();
        lineStripGround.model = glm::mat4(1);
        LS_draw(&lineStripGround, 2);

    }
    glEnable(GL_DEPTH_TEST);


    demo_render(framebuffer_width, framebuffer_height);

    glFlush();
}

void MapEditor::systemCallback_WindowResize(int win_width, int win_height)
{
    framebuffer_width = win_width;
    framebuffer_height = win_height;

    glViewport (0, 0, (GLsizei) framebuffer_width, (GLsizei) framebuffer_height);

    //    textRenderer_v2->onVievportResize(framebuffer_width, framebuffer_height);

    camera.onFrameBufferResize(framebuffer_width, framebuffer_height);
}

void MapEditor::systemCallback_Scroll(double yoffset){
    demo_onScrollCallback(yoffset);
    if(demo_isAnyWindowHovered()) //if input is on window, end process events
        return;

    camera.onScroll(yoffset);



    float x_ndc, y_ndc;
    get_ndc_coordinates(current_mouse_x_pos, current_mouse_y_pos, &x_ndc, &y_ndc);

    if(x_ndc > 0)
    {
        camera.changeXPosition(1.0f);
    }
    else
    {
        camera.changeXPosition(-1.0f);
    }

    if(y_ndc > 0)
    {
        camera.changeYPosition(1.0f);
    }
    else
    {
        camera.changeYPosition(-1.0f);
    }


}

void MapEditor::addGroundPointInFramebufferCoordinates(int framebuffer_x, int framebuffer_y)
{
    glm::vec3 world_position;
    windowCoordinatesToBoxCoordinates(framebuffer_x, framebuffer_y, world_position);
    dots.push_back(world_position);

    level.ground_verticles.push_back(world_position);
    LS_updateData(&lineStripGround,level.ground_verticles.data(), level.ground_verticles.size());
}

void MapEditor::addCoinInFramebufferCoordinates(int framebuffer_x, int framebuffer_y)
{
    glm::vec3 world_position;
    windowCoordinatesToBoxCoordinates(framebuffer_x, framebuffer_y, world_position);

    level.coins_vector.push_back(new CircleCoinRender(world_position.x, world_position.y, 0.25));

}

void MapEditor::systemCallback_mouseButton(SystemAbstraction::MouseButton mouseButton, SystemAbstraction::ButtonEvent event, int window_x, int window_y)
{
    LOGD("MapEditor::systemCallback_mouseButton()\n");
    current_mouse_x_pos = window_x;
    current_mouse_y_pos = window_y;

    demo_onMouseButtonCallback(mouseButton, event, window_x, window_y);
    if(demo_isAnyWindowHovered()) //if input is on window, end process events
        return;

    if ((mouseButton == SystemAbstraction::MOUSE_LEFT_BUTTON) && (event == SystemAbstraction::EVENT_DOWN))
    {
        windowCoordinatesToBoxCoordinates(window_x, window_y, touch_start_position_in_world);
    }
    else if((mouseButton == SystemAbstraction::MOUSE_LEFT_BUTTON) && (event == SystemAbstraction::EVENT_UP))
    {
        if(cursorMode == CURSOR_ADD_FANT)
        {
            if(fantMode == FANT_GROUND) {
                addGroundPointInFramebufferCoordinates(window_x, window_y);
            }else if(fantMode == FANT_COIN)
            {
                addCoinInFramebufferCoordinates(window_x, window_y);
            }
        }
        //if pointer in Move mode change to add fant
        cursorMode = CURSOR_ADD_FANT;
    }

}

void MapEditor::systemCallback_mouseMove(int x, int y)
{
    LOGD("MapEditor::systemCallback_mouseMove(%d, %d)", x, y);
    demo_onMouseMoveCallcack(x, y);
    if(demo_isAnyWindowHovered()) //if input is on window, end process events
        return;
}

void MapEditor::systemCallback_OnPointerDown(int pointerId, const struct PointerCoords *coords)
{
    demo_onMouseButtonCallback(SystemAbstraction::MOUSE_LEFT_BUTTON,
                               SystemAbstraction::EVENT_DOWN, (int) coords->x, (int) coords->y);
    if(demo_isAnyWindowHovered()) //if input is on window, end process events
        return;



    windowCoordinatesToBoxCoordinates(coords->x, coords->y, touch_start_position_in_world);
}

void MapEditor::systemCallback_OnPointerUp(int pointerId, const struct PointerCoords *coords)
{
    demo_onMouseButtonCallback(SystemAbstraction::MOUSE_LEFT_BUTTON,
                               SystemAbstraction::EVENT_UP, (int) coords->x, (int) coords->y);
    if(demo_isAnyWindowHovered()) //if input is on window, end process events
        return;

    if(cursorMode == CURSOR_ADD_FANT)
    {
        if(fantMode == FANT_GROUND) {
            addGroundPointInFramebufferCoordinates(coords->x, coords->y);
        }else if(fantMode == FANT_COIN)
        {
            addCoinInFramebufferCoordinates(coords->x, coords->y);
        }
    }
    //if pointer in Move mode change to add fant
    cursorMode = CURSOR_ADD_FANT;
}

void MapEditor::get_ndc_coordinates(float current_mouse_x_pos, float current_mouse_y_pos, float * x_ndc, float * y_ndc)
{
    *x_ndc = (current_mouse_x_pos/framebuffer_width)*2.0f - 1.0f;
    *y_ndc = -(current_mouse_y_pos/framebuffer_height)*2.0f + 1.0f;
}

void MapEditor::systemCallback_OnPointerMove(int pointerId, const struct PointerCoords *coords)
{
    demo_onPointerMoveCallback(pointerId, coords);
    if(demo_isAnyWindowHovered()) //if input is on window, end process events
        return;

    current_mouse_x_pos = coords->x;
    current_mouse_y_pos = coords->y;


    glm::vec3 touch_current_position_in_world;
    windowCoordinatesToBoxCoordinates(current_mouse_x_pos,current_mouse_y_pos,touch_current_position_in_world);


    float delta_cam_x = touch_current_position_in_world.x - touch_start_position_in_world.x;
    float delta_cam_y = touch_current_position_in_world.y - touch_start_position_in_world.y;

    if(delta_cam_x > 1.0f || delta_cam_y > 1.0f || delta_cam_x < -1.0f || delta_cam_y < -1.0f)
    {
        cursorMode = CURSOR_MOVE;
    }

    if(cursorMode == CURSOR_MOVE) {
        camera.changeXPosition(-delta_cam_x);
        camera.changeYPosition(-delta_cam_y);
    }

    redDotCursorModel = glm::translate(glm::mat4(1),glm::vec3(touch_current_position_in_world.x, touch_current_position_in_world.y, 0.0f));

}

void MapEditor::gui_onSaveMapButtonClicked()
{
    printf("MapEditor::gui_onSaveMapButtonClicked()\n");

    //LEVEL LOAD
    mapFilePath = getAppConfigFilePath() + "/CapitanAfrica.map";
    mapFileOpenErrno = level.saveLevelToFile(mapFilePath);
    if(mapFileOpenErrno)
    {
        mapFileOpenErrorString = strerror(mapFileOpenErrno);
    }
}

void MapEditor::gui_onClearMapButtonClicked()
{
    level.clear();
    dots.clear();
    LS_updateData(&lineStripGround,level.ground_verticles.data(), level.ground_verticles.size());
}

void MapEditor::gui_onCursorModeChanged(int mode)
{
    LOGD("MapEditor::gui_onCursorModeChanged(%d)", mode);
    fantMode = (FantMode)mode;
}

void MapEditor::systemCallback_OnChar(unsigned int codepoint)
{
    demo_onCharCallback(codepoint);
    if(demo_isAnyWindowHovered()) //if input is on window, end process events
        return;
}

void MapEditor::systemCallback_OnKey(SystemAbstraction::ButtonEvent event,SystemAbstraction:: Key key, SystemAbstraction::Mods mods, int x, int y)
{
    demo_onKeyCallback(event, key, mods, x, y);
    if(demo_isAnyWindowHovered()) //if input is on window, end process events
        return;

    if((key == 'd' || key == 'D') && (event == SystemAbstraction::EVENT_DOWN)){
        camera.changeXPosition(1.0f);
    }

    if((key == 'a' || key == 'A') && (event == SystemAbstraction:: EVENT_DOWN)){
        camera.changeXPosition(-1.0f);
    }

    if((key == 'w' || key == 'W') && (event == SystemAbstraction::EVENT_DOWN)){
        camera.changeYPosition(1.0f);
    }

    if((key == 's' || key == 'S') && (event == SystemAbstraction::EVENT_DOWN)){
        camera.changeYPosition(-1.0f);
    }


}

void MapEditor::windowCoordinatesToBoxCoordinates(double x_window, double y_window, glm::vec3 & world_position)
{
    float x_ndc = 0.0f;
    float y_ndc = 0.0f;

    get_ndc_coordinates(x_window, y_window, &x_ndc, &y_ndc);

    glm::mat4 PV_I = glm::inverse(camera.getProjectionMatrix()*camera.getViewMatrix());
    world_position = PV_I*glm::vec4(x_ndc, y_ndc, 0.0f, 1.0f);
    world_position.z = 0.0f;
}
