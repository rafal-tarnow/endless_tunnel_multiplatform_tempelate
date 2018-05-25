#include "map_editor.hpp"
#include <system_opengl_include.hpp>
#include <system_log.hpp>
#include <iostream>
#include <system_paths.hpp>
#include <system_log.hpp>
#include "demo.hpp"
#include "../system_abstraction.hpp"
#include "transformation.hpp"
#include <library_opengles_2/Resources/Resources.hpp>
#include <library_opengles_2/TextureManager/texture_manager.hpp>

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


    redDotTextureId = TextureManager::getTextureId("textures/red_dot.png");
    DE_initRectangle(&redDotPointerRectangle, &redDotTextureId,0.25f, 0.25f, 0.0f);

    gridLines = new CGridLines(0, 1000, 50, 0);


    textRenderer_v2 = new TextRenderer_v2(fb_width, fb_height);
    Resource font_design_graffiti_agentorange("fonts/design_graffiti_agentorange_www_myfontfree_com.ttf");

    textRenderer_v2->LoadFromMemory(font_design_graffiti_agentorange.getData(), font_design_graffiti_agentorange.getSize(), fb_height*0.06);

    //COORDINATES LINES
    glm::vec4 green_color(0.0f, 1.0f, 0.0f, 1.0f);
    float xlineVerticles[6] = {-1000.0f, 0.0f, 0.0f, 1000.0f, 0.0f, 0.0f};
    LS_init(&x_lineStrip,xlineVerticles,6, green_color);

    float ylineVerticles[6] = {0.0f, 1000.0f, 0.0f, 0.0f, -1000.0f, 0.0f};
    LS_init(&y_lineStrip,ylineVerticles,6, green_color);

    //LEVEL LOAD
    mapFilePath = getAppConfigFilePath() + "/CapitanAfrica.map";
    mapFileOpenErrno = level.loadLevelFromFile(mapFilePath);
    if(mapFileOpenErrno)
    {
        mapFileOpenErrorString = strerror(mapFileOpenErrno);
    }
    glm::vec4 red_color(1.0f, 0.0f, 0.0f, 1.0f);
    LS_init(&lineStripGround, level.ground_verticles.data(), level.ground_verticles.size(), red_color);

    mapEditorGui_setEventListener(this);
}

MapEditor::~MapEditor()
{
    mapEditorGui_setEventListener(nullptr);
    delete gridLines;
    DE_deleteRectangle(&redDotPointerRectangle);
    LS_delete(&x_lineStrip);
    LS_delete(&y_lineStrip);
}

void MapEditor::systemCallback_Render()
{
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

        if(level.meta != nullptr)
        {
            level.meta->render(camera.getProjectionMatrix(),camera.getViewMatrix());
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


    mapEditorGui_render(framebuffer_width, framebuffer_height);

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
    if(mapEditorGui_isAnyWindowHovered()) //if input is on window, end process events
        return;

    camera.onScroll(yoffset);



    float x_ndc, y_ndc;


    Transformation::Framebuffer framebuffer;
    framebuffer.framebuffer_width = framebuffer_width;
    framebuffer.framebuffer_height = framebuffer_height;

    Transformation::get_ndc_coordinates(framebuffer, current_mouse_x_pos, current_mouse_y_pos, &x_ndc, &y_ndc);

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
    fbCoordToWorldCoord(framebuffer_x, framebuffer_y, world_position);


    dots.push_back(world_position);

    level.ground_verticles.push_back(world_position);
    LS_updateData(&lineStripGround,level.ground_verticles.data(), level.ground_verticles.size());
}

void MapEditor::addMetaInFramebufferCoordinates(int framebuffer_x, int framebuffer_y)
{
    glm::vec3 world_position;
    fbCoordToWorldCoord(framebuffer_x, framebuffer_y, world_position);

    if(level.meta == nullptr)
    {
         level.meta = new MetaRenderer(world_position.x, world_position.y, -2.0f, 1.5);
    }
    else
    {
        level.meta->setPosAndDimm(world_position.x, world_position.y, -2.0f, 1.5);
    }
}

void MapEditor::addCoinInFramebufferCoordinates(int framebuffer_x, int framebuffer_y)
{
    glm::vec3 world_position;
    fbCoordToWorldCoord(framebuffer_x, framebuffer_y, world_position);

    level.coins_vector.push_back(new CircleCoinRender(world_position.x, world_position.y, -2.0f, 0.25));
}

void MapEditor::systemCallback_mouseButton(SystemAbstraction::MouseButton mouseButton, SystemAbstraction::ButtonEvent event, int window_x, int window_y)
{
    LOGD("MapEditor::systemCallback_mouseButton()\n");
    current_mouse_x_pos = window_x;
    current_mouse_y_pos = window_y;

    demo_onMouseButtonCallback(mouseButton, event, window_x, window_y);
    if(mapEditorGui_isAnyWindowHovered()) //if input is on window, end process events
        return;

    if ((mouseButton == SystemAbstraction::MOUSE_LEFT_BUTTON) && (event == SystemAbstraction::EVENT_DOWN))
    {
        leftMouseButtonIsPressed = true;
        fbCoordToWorldCoord(window_x, window_y, touch_start_position_in_world);
    }
    else if((mouseButton == SystemAbstraction::MOUSE_LEFT_BUTTON) && (event == SystemAbstraction::EVENT_UP))
    {
        leftMouseButtonIsPressed = false;

        if(cursorMode == CURSOR_ADD_FANT)
        {
            if(fantMode == FANT_GROUND) {
                addGroundPointInFramebufferCoordinates(window_x, window_y);
            }else if(fantMode == FANT_COIN)
            {
                addCoinInFramebufferCoordinates(window_x, window_y);
            }else if(fantMode == FANT_META){
                addMetaInFramebufferCoordinates(window_x, window_y);
            }
        }
        //if pointer in Move mode change to add fant
        cursorMode = CURSOR_ADD_FANT;
    }

}

void MapEditor::systemCallback_OnPointerUp(int pointerId, const struct PointerCoords *coords)
{
    demo_onMouseButtonCallback(SystemAbstraction::MOUSE_LEFT_BUTTON,
                               SystemAbstraction::EVENT_UP, (int) coords->x, (int) coords->y);
    if(mapEditorGui_isAnyWindowHovered()) //if input is on window, end process events
        return;

    if(cursorMode == CURSOR_ADD_FANT)
    {
        if(fantMode == FANT_GROUND) {
            addGroundPointInFramebufferCoordinates(coords->x, coords->y);
        }else if(fantMode == FANT_COIN)
        {
            addCoinInFramebufferCoordinates(coords->x, coords->y);
        }else if(fantMode == FANT_META)
        {
            addMetaInFramebufferCoordinates(coords->x, coords->y);
        }
    }
    //if pointer in Move mode change to add fant
    cursorMode = CURSOR_ADD_FANT;
}

void MapEditor::systemCallback_mouseMove(int x, int y)
{
    LOGD("MapEditor::systemCallback_mouseMove(%d, %d)", x, y);
    demo_onMouseMoveCallcack(x, y);
    if(mapEditorGui_isAnyWindowHovered()) //if input is on window, end process events
        return;

    if(leftMouseButtonIsPressed == false)
        return;

    current_mouse_x_pos = x;
    current_mouse_y_pos = y;


    glm::vec3 touch_current_position_in_world;
    fbCoordToWorldCoord(current_mouse_x_pos,current_mouse_y_pos,touch_current_position_in_world);


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

void MapEditor::systemCallback_OnPointerDown(int pointerId, const struct PointerCoords *coords)
{
    demo_onMouseButtonCallback(SystemAbstraction::MOUSE_LEFT_BUTTON,
                               SystemAbstraction::EVENT_DOWN, (int) coords->x, (int) coords->y);
    if(mapEditorGui_isAnyWindowHovered()) //if input is on window, end process events
        return;



    fbCoordToWorldCoord(coords->x, coords->y, touch_start_position_in_world);
}

void MapEditor::systemCallback_OnPointerMove(int pointerId, const struct PointerCoords *coords)
{
    demo_onPointerMoveCallback(pointerId, coords);
    if(mapEditorGui_isAnyWindowHovered()) //if input is on window, end process events
        return;

    current_mouse_x_pos = coords->x;
    current_mouse_y_pos = coords->y;


    glm::vec3 touch_current_position_in_world;
    fbCoordToWorldCoord(current_mouse_x_pos,current_mouse_y_pos,touch_current_position_in_world);


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
    if(mapEditorGui_isAnyWindowHovered()) //if input is on window, end process events
        return;
}

void MapEditor::systemCallback_OnKey(SystemAbstraction::ButtonEvent event,SystemAbstraction:: Key key, SystemAbstraction::Mods mods, int x, int y)
{
    demo_onKeyCallback(event, key, mods, x, y);
    if(mapEditorGui_isAnyWindowHovered()) //if input is on window, end process events
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

void MapEditor::fbCoordToWorldCoord(double window_x_pos, double window_y_pos, glm::vec3 & position_in_world)
{
    //    Transformation::Config config;
    //    config.framebuffer.framebuffer_height = framebuffer_height;
    //    config.framebuffer.framebuffer_width = framebuffer_width;
    //    config.projection = camera.getProjectionMatrix();
    //    config.view = camera.getViewMatrix();

    //    Transformation::framebufferCoordinatesToWorldCoordinates(config, window_x_pos, window_y_pos, position_in_world);

    glm::vec3 window_position(window_x_pos, framebuffer_height - window_y_pos, 0.0f);
    glm::vec4 viewport(0,0,framebuffer_width, framebuffer_height);

    position_in_world = glm::unProject(window_position,camera.getViewMatrix(), camera.getProjectionMatrix(),viewport);
}


