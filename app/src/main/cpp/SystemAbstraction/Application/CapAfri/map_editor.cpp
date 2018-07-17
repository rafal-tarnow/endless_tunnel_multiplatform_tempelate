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

    glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
    glEnable (GL_BLEND);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    redDotTextureId = TextureManager::getTextureId("textures/red_dot.png");
    DE_initRectangle(&redDotPointerRectangle, &redDotTextureId,0.25f, 0.25f, 0.0f);

    yellowDotTextureId = TextureManager::getTextureId("textures/yellow_dot.png");
    DE_initRectangle(&yellowDotRectangle, &yellowDotTextureId, 0.25f, 0.25f, 0.0f);

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
    mapFilePath.str("");
    mapFilePath.clear();
    mapFilePath << getStandardCommonReadWriteDirecory() + "/CapitanAfrica_" << currentMapIndex << ".map" ;

    loadMap(mapFilePath.str());

    carRenderer = new CarRenderer(glm::vec3(0,0,0));

    mapEditorGui_setEventListener(this);


    yellowDotIndex = level.ground_verticles.end();
}


void MapEditor::loadMap(string mapFilePath)
{
    mapFileOpenErrno = level.loadLevelFromFile(mapFilePath);
    if(mapFileOpenErrno)
    {
        mapFileOpenErrorString = strerror(mapFileOpenErrno);
    }
    glm::vec4 red_color(1.0f, 0.0f, 0.0f, 1.0f);
    LS_init(&lineStripGround, level.ground_verticles.data(), level.ground_verticles.size(), red_color);
}

MapEditor::~MapEditor()
{
    string configFilePath = getStandardCommonReadWriteDirecory() + "/CapitanAfrica.config";
    Config config;
    config.loadDataFromFileToMemory(configFilePath);
    config.set_uint32_t("currentMapIndex",currentMapIndex);
    config.saveDataFromMemoryToFile(configFilePath);

    mapEditorGui_setEventListener(nullptr);
    delete gridLines;
    DE_deleteRectangle(&redDotPointerRectangle);
    DE_deleteRectangle(&yellowDotRectangle);
    LS_delete(&x_lineStrip);
    LS_delete(&y_lineStrip);

    delete carRenderer;
}

void MapEditor::systemCallback_Render()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if(mapFileOpenErrno != 0)
    {
        textRenderer_v2->RenderText("ERROR WHILE OPEN FILE:",framebuffer_width*0.1,framebuffer_height*0.6);
        textRenderer_v2->RenderText(mapFilePath.str(),framebuffer_width*0.1,framebuffer_height*0.5);
        textRenderer_v2->RenderText(mapFileOpenErrorString,framebuffer_width*0.1,framebuffer_height*0.4);
        return;
    }

    glDisable(GL_DEPTH_TEST);
    {
        glEnable (GL_BLEND);
        glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        //DRAW CAR
        glm::vec3 camPosition = camera.getPosition();
        camPosition.y += camera.getViewHeight()*0.5*0.7;
        carRenderer->setPosition(camPosition);
        carRenderer->render(camera.getProjectionMatrix(), camera.getViewMatrix());

        //DRAW GRID LINES
        glLineWidth(1.0);

        glm::mat4 PVM = camera.getProjectionMatrix()*camera.getViewMatrix()*glm::mat4(1);
        gridLines->Render(glm::value_ptr(PVM), glm::value_ptr(glm::mat4(1)), glm::value_ptr(glm::mat4(1)));

        redDotPointerRectangle.projection = camera.getProjectionMatrix();
        redDotPointerRectangle.view = camera.getViewMatrix();
        redDotPointerRectangle.model = redDotCursorModel;
        DE_drawRectangle(&redDotPointerRectangle);


        for(auto i = level.ground_verticles.begin(); i != level.ground_verticles.end(); i++)
        {
            glm::mat4 model = glm::translate(glm::mat4(1), *i);
            redDotPointerRectangle.model = model;
            DE_drawRectangle(&redDotPointerRectangle);
        }

        yellowDotRectangle.projection = camera.getProjectionMatrix();
        yellowDotRectangle.view = camera.getViewMatrix();
        if((cursorMode == CURSOR_MOVE_ELEMENT) && (yellowDotIndex != level.ground_verticles.end()))
            yellowDotRectangle.model = glm::translate(glm::mat4(1), *yellowDotIndex);
        DE_drawRectangle(&yellowDotRectangle);

        for (auto & coin : level.coins_vector){
            coin->render(camera.getProjectionMatrix(),camera.getViewMatrix());
        }


        if(level.mushroom_vector.size() > 0)
        {
            for( auto & mushroom : level.mushroom_vector)
            {
                mushroom->render(camera.getProjectionMatrix(), camera.getViewMatrix());
            }
        }

        if(level.meta != nullptr)
        {
            level.meta->render(camera.getProjectionMatrix(),camera.getViewMatrix());
        }


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

    updateGuiDebugInfo();

    mapEditorGui_render(framebuffer_width, framebuffer_height);

    glFlush();
}

void MapEditor::updateGuiDebugInfo()
{
    string fantModeTxt;
    string cursorModeTxt;

    switch (cursorMode) {
    case CURSOR_MOVE_CAMERA:
        cursorModeTxt = "CURSOR_MOVE_CAMERA";
        break;
    case CURSOR_ADD_FANT:
        cursorModeTxt = "CURSOR_ADD_FANT";
        break;
    case CURSOR_ZOOM:
        cursorModeTxt = "CURSOR_ZOOM";
        break;
    case CURSOR_MOVE_ELEMENT:
        cursorModeTxt = "CURSOR_MOVE_ELEMENT";
        break;
    case CURSOR_MOVE_ELEMENT_MOVE_CAMERA:
        cursorModeTxt = "CURSOR_MOVE_ELEMENT_MOVE_CAMERA";
        break;
    case CURSOR_MOVE_ELEMENT_ZOOM:
        cursorModeTxt = "CURSOR_MOVE_ELEMENT_ZOOM";
        break;
    }

    switch (fantMode) {
    case FANT_GROUND:
        fantModeTxt = "FANT_GROUND";
        break;
    case FANT_COIN:
        fantModeTxt = "FANT_COIN";
        break;
    case FANT_MUSHROOM:
        fantModeTxt = "FANT_MUSHROOM";
        break;
    case FANT_META:
        fantModeTxt = "FANT_META";
        break;
    }

    demo_setCursorModeText_dbg(cursorModeTxt);
    demo_setFantModeText_dbg(fantModeTxt);

}

void MapEditor::systemCallback_WindowResize(int win_width, int win_height)
{
    framebuffer_width = win_width;
    framebuffer_height = win_height;

    glViewport (0, 0, (GLsizei) framebuffer_width, (GLsizei) framebuffer_height);

    //    textRenderer_v2->onVievportResize(framebuffer_width, framebuffer_height);

    camera.onFrameBufferResize(framebuffer_width, framebuffer_height);
}

void MapEditor::systemCallback_MouseScroll(double yoffset){
    demo_onScrollCallback(yoffset);
    if(mapEditorGui_isAnyWindowHovered()) //if input is on window, end process events
        return;

    cout << "yoffset = " << yoffset << endl;

    if(yoffset > 0)
    {
        camera.zoomIn();
    }
    else
    {
        camera.zoomOut();
    }

}

bool myfunction (glm::vec3 &i,glm::vec3  &j) { return (i.x<j.x); }

void MapEditor::addGroundPointInFramebufferCoordinates()
{
    level.ground_verticles.push_back(world_current_position_0);
    std::sort (level.ground_verticles.begin(), level.ground_verticles.end(), myfunction);

    LS_updateData(&lineStripGround,level.ground_verticles.data(), level.ground_verticles.size());
}

void MapEditor::addMetaInFramebufferCoordinates()
{
    if(level.meta == nullptr)
    {
        level.meta = new MetaRenderer(glm::vec3(world_current_position_0.x, world_current_position_0.y, -2.0f), 1.5);
    }
    else
    {
        level.meta->setPosAndDimm(glm::vec3(world_current_position_0.x, world_current_position_0.y, -2.0f), 1.5);
    }
}

void MapEditor::addCoinInFramebufferCoordinates()
{
    level.coins_vector.push_back(new CircleCoinRender(world_current_position_0.x, world_current_position_0.y, -2.0f, 0.25));
}

void MapEditor::addMushroomInFramebufferCoordinates()
{
    glm::vec2 dimm(2.0, 2.0);
    level.mushroom_vector.push_back(new MushroomRenderer(world_current_position_0));
}

void MapEditor::systemCallback_mouseButton(SystemAbstraction::MouseButton mouseButton, SystemAbstraction::ButtonEvent event, int window_x, int window_y)
{

}

void MapEditor::systemCallback_mouseMove(int x, int y)
{

}

void MapEditor::systemCallback_OnPointerDown(int pointerId, const struct PointerCoords *coords)
{
    LOGD("onPointerDown %d",pointerId);

    demo_onMouseButtonCallback(SystemAbstraction::MOUSE_LEFT_BUTTON,
                               SystemAbstraction::EVENT_DOWN, (int) coords->x, (int) coords->y);
    if(mapEditorGui_isAnyWindowHovered()) //if input is on window, end process events
        return;


    if(pointerId == 0)
    {
        pointer_0_start_coords = *coords;
        pointer_0_current_coords = *coords;

        fbCoordToWorldCoord(coords->x, coords->y, world_start_position_0);
        fbCoordToWorldCoord(coords->x, coords->y, world_current_position_0);
    }
    else if(pointerId == 1)
    {
        pointer_1_start_coords = *coords;
        pointer_1_current_coords = *coords;

        fbCoordToWorldCoord(coords->x, coords->y, world_start_position_1);
        fbCoordToWorldCoord(coords->x, coords->y, world_current_position_1);
    }


    switch (cursorMode)
    {
    case CURSOR_MOVE_CAMERA:
        if(pointerId == 1)
        {
            cursorMode = CURSOR_ZOOM;

            start_zoom_distance = glm::distance(world_current_position_1, world_current_position_0);
            start_camera_zoom = camera.getZoom();
            return;
        }
        break;
    case CURSOR_ADD_FANT:
        if(pointerId == 1)
        {
            cursorMode = CURSOR_ZOOM;

            start_zoom_distance = glm::distance(world_current_position_1, world_current_position_0);
            start_camera_zoom = camera.getZoom();
            return;
        }
        break;

    case CURSOR_ZOOM:

        break;

    case CURSOR_MOVE_ELEMENT:
        if(pointerId == 0)
        {

            float minimalDistance;
            auto minimalDistanceIndex = level.ground_verticles.begin();

            for(auto i = level.ground_verticles.begin(); i != level.ground_verticles.end(); i++)
            {

                float distance = glm::distance(world_current_position_0, *i);

                if(i == level.ground_verticles.begin())
                {
                    minimalDistance = distance;
                    minimalDistanceIndex = i;
                }
                else
                {
                    if(distance < minimalDistance)
                    {
                        minimalDistance = distance;
                        minimalDistanceIndex = i;
                    }
                }
            }

            if(minimalDistance < 1.0)
            {
                yellowDotIndex = minimalDistanceIndex;
            }
            else
            {
                cursorMode = CURSOR_MOVE_ELEMENT_MOVE_CAMERA;
                fbCoordToWorldCoord(coords->x, coords->y, world_move_camera_position_start);
            }

        }
        break;
    case CURSOR_MOVE_ELEMENT_MOVE_CAMERA:
        if(pointerId == 1)
        {
            cursorMode = CURSOR_MOVE_ELEMENT_ZOOM;

            start_zoom_distance = glm::distance(world_current_position_1, world_current_position_0);
            start_camera_zoom = camera.getZoom();

            //world_zoom_start_middle_position =
        }
        break;
    }
}

void MapEditor::systemCallback_OnPointerMove(int pointerId, const struct PointerCoords *coords)
{
    LOGD("onPointerMove %d",pointerId);

    if(pointerId == 0)
    {
        pointer_0_current_coords = *coords;
        fbCoordToWorldCoord(coords->x, coords->y, world_current_position_0);
    }
    else if(pointerId == 1)
    {
        pointer_1_current_coords = *coords;
        fbCoordToWorldCoord(coords->x, coords->y, world_current_position_1);
    }


    demo_onPointerMoveCallback(pointerId, coords);
    if(mapEditorGui_isAnyWindowHovered()) //if input is on window, end process events
        return;


    switch (cursorMode) {
    case CURSOR_MOVE_CAMERA:
        if(pointerId == 0)
        {

            float delta_cam_x = world_current_position_0.x - world_move_camera_position_start.x;
            float delta_cam_y = world_current_position_0.y - world_move_camera_position_start.y;

            camera.changeXPosition(-delta_cam_x);
            camera.changeYPosition(-delta_cam_y);

            redDotCursorModel = glm::translate(glm::mat4(1),glm::vec3(world_current_position_0.x, world_current_position_0.y, 0.0f));
        }
        break;
    case CURSOR_ADD_FANT:
        if(pointerId == 0)
        {

            float delta_cam_x = world_current_position_0.x - world_start_position_0.x;
            float delta_cam_y = world_current_position_0.y - world_start_position_0.y;

            if(delta_cam_x > 1.0f*camera.getZoom() || delta_cam_y > 1.0f*camera.getZoom() || delta_cam_x < -1.0f*camera.getZoom() || delta_cam_y < -1.0f*camera.getZoom())
            {
                cursorMode = CURSOR_MOVE_CAMERA;
                fbCoordToWorldCoord(pointer_0_current_coords.x,pointer_0_current_coords.y, world_move_camera_position_start);
            }

            redDotCursorModel = glm::translate(glm::mat4(1),glm::vec3(world_current_position_0.x, world_current_position_0.y, 0.0f));
        }
        break;

    case CURSOR_ZOOM:
        if(pointerId == 0 || pointerId == 1)
        {
            float current_distance = glm::distance(world_current_position_1, world_current_position_0);
            camera.setZoom(start_camera_zoom*(start_zoom_distance/current_distance));
        }
        break;

    case CURSOR_MOVE_ELEMENT:
        if(pointerId == 0)
        {
            *yellowDotIndex = world_current_position_0;
            std::sort (level.ground_verticles.begin(), level.ground_verticles.end(), myfunction);
            LS_updateData(&lineStripGround,level.ground_verticles.data(), level.ground_verticles.size());
        }
        break;

    case CURSOR_MOVE_ELEMENT_MOVE_CAMERA:
        if(pointerId == 0)
        {
            float delta_cam_x = world_current_position_0.x - world_move_camera_position_start.x;
            float delta_cam_y = world_current_position_0.y - world_move_camera_position_start.y;

            camera.changeXPosition(-delta_cam_x);
            camera.changeYPosition(-delta_cam_y);
        }
            if(pointerId == 1)
            {
                float delta_cam_x = world_current_position_1.x - world_move_camera_position_start.x;
                float delta_cam_y = world_current_position_1.y - world_move_camera_position_start.y;

                camera.changeXPosition(-delta_cam_x);
                camera.changeYPosition(-delta_cam_y);
            }
        break;
    case CURSOR_MOVE_ELEMENT_ZOOM:
        if(pointerId == 0 || pointerId == 1)
        {
            float current_distance = glm::distance(world_current_position_1, world_current_position_0);
            camera.setZoom(start_camera_zoom*(start_zoom_distance/current_distance));
        }
        break;
    }





}

void MapEditor::systemCallback_OnPointerUp(int pointerId, const struct PointerCoords *coords)
{
    LOGD("onPointerUp %d",pointerId);
    if(pointerId == 0)
    {
        pointer_0_current_coords = *coords;
        pointer_0_stop_coords = *coords;

        fbCoordToWorldCoord(coords->x, coords->y, world_current_position_0);
        fbCoordToWorldCoord(coords->x, coords->y, world_stop_position_0);
    }
    else if(pointerId == 1)
    {
        pointer_1_current_coords = *coords;
        pointer_1_stop_coords = *coords;

        fbCoordToWorldCoord(coords->x, coords->y, world_current_position_1);
        fbCoordToWorldCoord(coords->x, coords->y, world_stop_position_1);
    }
    
    demo_onMouseButtonCallback(SystemAbstraction::MOUSE_LEFT_BUTTON,
                               SystemAbstraction::EVENT_UP, (int) coords->x, (int) coords->y);
    if(mapEditorGui_isAnyWindowHovered()) //if input is on window, end process events
        return;
    


    switch (cursorMode) {
    case CURSOR_MOVE_CAMERA:
        cursorMode = CURSOR_ADD_FANT;
        break;
    case CURSOR_ADD_FANT:
        if(fantMode == FANT_GROUND) {
            addGroundPointInFramebufferCoordinates();
        }
        else if(fantMode == FANT_COIN)
        {
            addCoinInFramebufferCoordinates();
        }
        else if(fantMode == FANT_MUSHROOM)
        {
            addMushroomInFramebufferCoordinates();
        }
        else if(fantMode == FANT_META)
        {
            addMetaInFramebufferCoordinates();
        }
        break;

    case CURSOR_ZOOM:
        if(pointerId == 0)
        {
            cursorMode = CURSOR_MOVE_CAMERA;
            fbCoordToWorldCoord(pointer_1_current_coords.x, pointer_1_current_coords.y, world_move_camera_position_start);
        }
        else if(pointerId == 1)
        {
            cursorMode = CURSOR_MOVE_CAMERA;
            fbCoordToWorldCoord(pointer_0_current_coords.x, pointer_0_current_coords.y, world_move_camera_position_start);
        }
        break;

    case CURSOR_MOVE_ELEMENT:

        break;
    case CURSOR_MOVE_ELEMENT_MOVE_CAMERA:
        if(pointerId == 0 || pointerId == 1)
        {
            cursorMode = CURSOR_MOVE_ELEMENT;
        }
        break;
    case CURSOR_MOVE_ELEMENT_ZOOM:
        if(pointerId == 0)
        {
            cursorMode = CURSOR_MOVE_ELEMENT_MOVE_CAMERA;
            fbCoordToWorldCoord(pointer_1_current_coords.x, pointer_1_current_coords.y, world_move_camera_position_start);
        }
        else if(pointerId == 1)
        {
            cursorMode = CURSOR_MOVE_ELEMENT_MOVE_CAMERA;
            fbCoordToWorldCoord(pointer_0_current_coords.x, pointer_0_current_coords.y, world_move_camera_position_start);
        }
        break;
    }



}

void MapEditor::gui_onSaveMapButtonClicked()
{
    printf("MapEditor::gui_onSaveMapButtonClicked()\n");

    //LEVEL LOAD
    mapFilePath.str("");
    mapFilePath.clear();
    mapFilePath << getStandardCommonReadWriteDirecory() << "/CapitanAfrica_" << currentMapIndex <<  ".map";
    mapFileOpenErrno = level.saveLevelToFile(mapFilePath.str());
    if(mapFileOpenErrno)
    {
        mapFileOpenErrorString = strerror(mapFileOpenErrno);
    }
}

void MapEditor::gui_onClearMapButtonClicked()
{
    level.clear();
    LS_updateData(&lineStripGround,level.ground_verticles.data(), level.ground_verticles.size());
}

void MapEditor::gui_onCursorModeChanged(int mode)
{
    LOGD("MapEditor::gui_onCursorModeChanged(%d)", mode);
    if(mode <= 3)
    {
        fantMode = (FantMode)mode;
        cursorMode = CURSOR_ADD_FANT;
    }
    else if(mode == 4)
    {
        cursorMode = CURSOR_MOVE_ELEMENT;
        yellowDotIndex = level.ground_verticles.end();
    }
}

void MapEditor::gui_onCurrentMapChanged(unsigned int currentMap)
{
    currentMapIndex = currentMap;

    mapFilePath.str("");
    mapFilePath.clear();
    mapFilePath << getStandardCommonReadWriteDirecory() + "/CapitanAfrica_" << currentMapIndex << ".map" ;

    loadMap(mapFilePath.str());
}

void MapEditor::gui_onZoomOut()
{
    camera.zoomOut();
}

void MapEditor::gui_onZoomIn()
{
    camera.zoomIn();
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
    glm::vec3 window_position(window_x_pos, framebuffer_height - window_y_pos, 0.0f);
    glm::vec4 viewport(0,0,framebuffer_width, framebuffer_height);

    position_in_world = glm::unProject(window_position,camera.getViewMatrix(), camera.getProjectionMatrix(),viewport);
    position_in_world.z = 0;
}


