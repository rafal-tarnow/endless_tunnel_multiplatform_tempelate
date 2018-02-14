#include "map_editor.hpp"
#include <system_opengl_include.hpp>
#include <system_log.hpp>
#include <iostream>
#include <SOIL.h>
#include "./data/red_dot.png.hpp"
#include <system_paths.hpp>
#include <system_log.hpp>
#include "design_graffiti_agentorange_www_myfontfree_com.ttf.hpp"

using namespace std;

MapEditor::MapEditor(int win_width, int win_height)
{
    current_window_width = win_width;
    current_window_height = win_height;
    systemCallback_WindowResize(current_window_width, current_window_height);

    //glEnable(GL_MULTISAMPLE);
    glEnable (GL_BLEND);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glClearColor(0.0,0.0,0.0,1.0);

    redDotTextureId = SOIL_load_OGL_texture_from_memory(red_dot_png, size_of_red_dot_png, 4,0,SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y);
    DE_initRectangle(&redDotPointerRectangle, &redDotTextureId,0.25f, 0.25f, 0.0f);

    gridLines = new CGridLines(0, 1000, 50, 0);

    cameraViewMatrix = glm::lookAt(glm::vec3(0,0, 10.0f),glm::vec3(0, 0, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

    textRenderer_v2 = new TextRenderer_v2(win_width, win_height);
    textRenderer_v2->LoadFromMemory(design_graffiti_agentorange_www_myfontfree_com_ttf, size_of_design_graffiti_agentorange_www_myfontfree_com_ttf, win_height*0.06);

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

    //GROUND LINE
    //    float verticles[6] = {1.0f, 1.0f, 0.0f, 3.0f, 2.0f, 0.0f};
    //    LS_initLineStrip(&lineStripGround, verticles, 6);

    LS_init(&lineStripGround, level.ground_verticles.data(), level.ground_verticles.size());
}

MapEditor::~MapEditor()
{
    delete gridLines;
    DE_deleteRectangle(&redDotPointerRectangle);
    LS_delete(&x_lineStrip);
    LS_delete(&y_lineStrip);
    glDeleteTextures(1,&redDotTextureId);
}

void MapEditor::systemCallback_Render()
{

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    if(mapFileOpenErrno != 0)
    {
        textRenderer_v2->RenderText("ERROR WHILE OPEN FILE:",current_window_width*0.1,current_window_height*0.6);
        textRenderer_v2->RenderText(mapFilePath,current_window_width*0.1,current_window_height*0.5);
        textRenderer_v2->RenderText(mapFileOpenErrorString,current_window_width*0.1,current_window_height*0.4);
        return;
    }

    glDisable(GL_DEPTH_TEST);
    {

        glLineWidth(1.0);

        glm::mat4 PVM = cameraProjectionMatrix*cameraViewMatrix*glm::mat4(1);

        gridLines->Render(glm::value_ptr(PVM), glm::value_ptr(glm::mat4(1)), glm::value_ptr(glm::mat4(1)));

        redDotPointerRectangle.projection = cameraProjectionMatrix;
        redDotPointerRectangle.view = cameraViewMatrix;
        redDotPointerRectangle.model = redDotCursorModel;

        DE_drawRectangle(&redDotPointerRectangle);

        if(dots.empty())
        {
            glm::vec3 point = glm::vec3(0.5f, 0.5f ,0.0f);
            dots.push_back(point);
        }

        for(unsigned int i = 0; i < dots.size(); i++)
        {
            glm::mat4 model = glm::translate(glm::mat4(1),dots[i]);
            redDotPointerRectangle.model = model;
            DE_drawRectangle(&redDotPointerRectangle);
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

        //    textRenderer_v2->RenderText(text.str(), current_window_width - 200.0f, current_window_height - 50.0f);


        //DRAW COORDINATES LINES
        x_lineStrip.projection = cameraProjectionMatrix;
        x_lineStrip.view = cameraViewMatrix;
        x_lineStrip.model = glm::mat4(1);
        LS_draw(&x_lineStrip, 2);

        y_lineStrip.projection = cameraProjectionMatrix;
        y_lineStrip.view = cameraViewMatrix;
        y_lineStrip.model = glm::mat4(1);
        LS_draw(&y_lineStrip, 2);

        //DRAW GROUND LINE
        lineStripGround.projection = cameraProjectionMatrix;
        lineStripGround.view = cameraViewMatrix;
        lineStripGround.model = glm::mat4(1);
        LS_draw(&lineStripGround, 3);

    }
    glEnable(GL_DEPTH_TEST);

    glFlush();
}

void MapEditor::systemCallback_WindowResize(int win_width, int win_height)
{
    current_window_width = win_width;
    current_window_height = win_height;
    glViewport (0, 0, (GLsizei) current_window_width, (GLsizei) current_window_height);

    //    textRenderer_v2->onVievportResize(current_window_width, current_window_height);

    float aspect = ((float)current_window_width)/((float)current_window_height);

    box_view_width_in_meters = 30.0f*zoom;
    box_view_height_in_meters = box_view_width_in_meters/aspect;
    cameraProjectionMatrix = glm::ortho(-box_view_width_in_meters/2.0f, box_view_width_in_meters/2.0f, -box_view_height_in_meters/2.0f, box_view_height_in_meters/2.0f ,-1000.0f,1000.0f);
}

void MapEditor::systemCallback_Scroll(double yoffset){
    if(yoffset > 0)
    {
        zoom = zoom / 1.2;
        if(zoom < 0.01)
            zoom = 0.01;
    }
    else
    {
        zoom = zoom * 1.2;
        if(zoom > 50.0)
            zoom = 50.0;
    }

    float aspect = ((float)current_window_width)/((float)current_window_height);

    box_view_width_in_meters = 30.0f*zoom;
    box_view_height_in_meters = box_view_width_in_meters/aspect;




    cameraProjectionMatrix = glm::ortho(-box_view_width_in_meters/2.0f, box_view_width_in_meters/2.0f, -box_view_height_in_meters/2.0f, box_view_height_in_meters/2.0f ,-1000.0f,1000.0f);


    float x_ndc, y_ndc;
    get_ndc_coordinates(current_mouse_x_pos, current_mouse_y_pos, &x_ndc, &y_ndc);

    if(x_ndc > 0)
    {
        camera_position_x += 1;
    }
    else
    {
        camera_position_x -= 1;
    }

    if(y_ndc > 0)
    {
        camera_position_y += 1;
    }
    else
    {
        camera_position_y -= 1;
    }
    updateCameraViewMatrix();
}

void MapEditor::systemCallback_mouseButton(SystemAbstraction::MouseButton mouseButton, SystemAbstraction::ButtonEvent event, int window_x, int window_y)
{
    LOGD("MapEditor::systemCallback_mouseButton()\n");
    current_mouse_x_pos = window_x;
    current_mouse_y_pos = window_y;


    if ((mouseButton == SystemAbstraction::MOUSE_LEFT_BUTTON) && (event == SystemAbstraction::EVENT_DOWN))
    {
        glm::vec3 world_position;
        windowCoordinatesToBoxCoordinates(window_x, window_y, world_position);
        dots.push_back(world_position);

        level.ground_verticles.push_back(world_position);
        LS_updateData(&lineStripGround,level.ground_verticles.data(), level.ground_verticles.size());
    }
    else if((mouseButton == SystemAbstraction::MOUSE_RIGHT_BUTTON) && (event == SystemAbstraction::EVENT_DOWN))
    {

    }


}

void MapEditor::get_ndc_coordinates(float current_mouse_x_pos, float current_mouse_y_pos, float * x_ndc, float * y_ndc)
{
    *x_ndc = (current_mouse_x_pos/current_window_width)*2.0f - 1.0f;
    *y_ndc = -(current_mouse_y_pos/current_window_height)*2.0f + 1.0f;
}

void MapEditor::systemCallback_mousePosition(double x, double y)
{
    current_mouse_x_pos = x;
    current_mouse_y_pos = y;


    glm::vec3 world_position;
    windowCoordinatesToBoxCoordinates(x,y,world_position);

    redDotCursorModel = glm::translate(glm::mat4(1),glm::vec3(world_position.x, world_position.y, 0.0f));
}

void MapEditor::systemCallback_keyboard(SystemAbstraction::ButtonEvent event, unsigned int key, int x, int y )
{

    if((key == 'd' || key == 'D') && (event == SystemAbstraction::EVENT_DOWN)){
        camera_position_x += 1.0f;
    }

    if((key == 'a' || key == 'A') && (event == SystemAbstraction:: EVENT_DOWN)){
        camera_position_x -= 1.0f;
    }

    if((key == 'w' || key == 'W') && (event == SystemAbstraction::EVENT_DOWN)){
        camera_position_y += 1.0f;
    }

    if((key == 's' || key == 'S') && (event == SystemAbstraction::EVENT_DOWN)){
        camera_position_y -= 1.0f;
    }

    updateCameraViewMatrix();
}

void MapEditor::updateCameraViewMatrix()
{
    cameraViewMatrix = glm::lookAt(glm::vec3(camera_position_x, camera_position_y, 10.0f),glm::vec3(camera_position_x, camera_position_y, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
}

void MapEditor::windowCoordinatesToBoxCoordinates(double x_window, double y_window, glm::vec3 & world_position)
{   
    float x_ndc = 0.0f;
    float y_ndc = 0.0f;

    get_ndc_coordinates(x_window, y_window, &x_ndc, &y_ndc);

    glm::mat4 PV_I = glm::inverse(cameraProjectionMatrix*cameraViewMatrix);
    world_position = PV_I*glm::vec4(x_ndc, y_ndc, 0.0f, 1.0f);
    world_position.z = 0.0f;
}