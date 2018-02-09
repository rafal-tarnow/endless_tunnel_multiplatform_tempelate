#include "map_editor.hpp"
#include <system_opengl_include.hpp>
#include <iostream>
#include <SOIL.h>
#include "./data/red_dot.png.hpp"
#include <system_paths.hpp>
#include "design_graffiti_agentorange_www_myfontfree_com.ttf.hpp"

using namespace std;

MapEditor::MapEditor(int win_width, int win_height)
{
    current_window_width = win_width;
    current_window_height = win_height;

    //glEnable(GL_MULTISAMPLE);
    glEnable (GL_BLEND);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glClearColor(0.0,0.0,0.0,1.0);

    DE_initShader();
    redDotTextureId = SOIL_load_OGL_texture_from_memory(red_dot_png, size_of_red_dot_png, 4,0,SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y);
    DE_initRectangle(&testRectangle, &redDotTextureId,0.25f, 0.25f, 0.0f);

    gridLines = new CGridLines(0, 1000, 50, 0);

    viewMatrix = glm::lookAt(glm::vec3(0,0, 10.0f),glm::vec3(0, 0, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

    textRenderer_v2 = new TextRenderer_v2(win_width, win_height);
    textRenderer_v2->LoadFromMemory(design_graffiti_agentorange_www_myfontfree_com_ttf, size_of_design_graffiti_agentorange_www_myfontfree_com_ttf, win_height*0.06);


    mapFilePath = getStandardCommonReadWriteDirecory() + "/CapitanAfrica.map";

    FILE * mapFile = fopen(mapFilePath.c_str(), "ab+");

    if(mapFile == NULL)
    {
        mapFileOpenErrno = errno;
        mapFileOpenErrorString = strerror(mapFileOpenErrno);
    }
    else
    {
        fclose(mapFile);
    }
}

MapEditor::~MapEditor()
{
    delete gridLines;
    DE_deleteRectangle(&testRectangle);
    glDeleteTextures(1,&redDotTextureId);
}

glm::mat4 rectModel = glm::mat4(1);

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

    glLineWidth(1.0);

    glm::mat4 PVM = projectionMatrix*viewMatrix*glm::mat4(1);

    gridLines->Render(glm::value_ptr(PVM), glm::value_ptr(glm::mat4(1)), glm::value_ptr(glm::mat4(1)));

    testRectangle.projection = projectionMatrix;
    testRectangle.view = viewMatrix;
    testRectangle.model = rectModel;

    DE_drawRectangle(&testRectangle);





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
    projectionMatrix = glm::ortho(-box_view_width_in_meters/2.0f, box_view_width_in_meters/2.0f, -box_view_height_in_meters/2.0f, box_view_height_in_meters/2.0f ,-1000.0f,1000.0f);
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
    projectionMatrix = glm::ortho(-box_view_width_in_meters/2.0f, box_view_width_in_meters/2.0f, -box_view_height_in_meters/2.0f, box_view_height_in_meters/2.0f ,-1000.0f,1000.0f);
}

void MapEditor::systemCallback_mouseButton(MouseButton mouseButton, SystemAbstraction::ButtonEvent event, int window_x, int window_y)
{
    cout << " MapEditor::systemCallback_mouseButton(...)" << endl;
    //float world_x, world_y;
    // windowCoordinatesToBoxCoordinates(window_x, window_y, world_x, world_y);

    if ((mouseButton == MOUSE_LEFT_BUTTON) && (event == SystemAbstraction::EVENT_DOWN))
    {


    }else if((mouseButton == MOUSE_RIGHT_BUTTON) && (event == SystemAbstraction::EVENT_DOWN))
    {

    }
}

void MapEditor::systemCallback_mousePosition(double x, double y)
{
    current_mouse_x_pos = x;
    current_mouse_y_pos = y;
    cout << "MapEditor::systemCallback_mousePosition(...) x = " << x << " y = " << y << endl;
    float x_ndc = 0.0f;
    float y_ndc = 0.0f;

    x_ndc = (x/current_window_width)*2.0f - 1.0f;
    y_ndc = -(y/current_window_height)*2.0f + 1.0f;

    cout << "x_ndc = " << x_ndc << " y_ndc = " << y_ndc << endl;


    glm::mat4 PV_I = glm::inverse(projectionMatrix*viewMatrix);

    glm::vec4 world_position = PV_I*glm::vec4(x_ndc, y_ndc, 0.0f, 1.0f);

    //    float x_world, y_world;
    //    windowCoordinatesToBoxCoordinates(x,y,x_world, y_world);

    cout << "World coordinates x_world = " << world_position.x << " y_world = " << world_position.y << endl;

    rectModel = glm::translate(glm::mat4(1),glm::vec3(world_position.x, world_position.y, 0.0f));


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

      viewMatrix = glm::lookAt(glm::vec3(camera_position_x, camera_position_y, 10.0f),glm::vec3(camera_position_x, camera_position_y, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
}

void MapEditor::windowCoordinatesToBoxCoordinates(double x, double y, float &x_out, float &y_out)
{
    float x_in = (float)x;
    float y_in = (float)y;

    float szerokosc_okna_do_szerokosci_swiata = current_window_width / box_view_width_in_meters;
    float wysokosc_okna_do_wysokosci_swiata = current_window_height / box_view_height_in_meters;

    x_out = (x_in / szerokosc_okna_do_szerokosci_swiata);// - camera_x_position_m;
    y_out = ((current_window_height - y_in) / wysokosc_okna_do_wysokosci_swiata);// - camera_y_position_m;
}
