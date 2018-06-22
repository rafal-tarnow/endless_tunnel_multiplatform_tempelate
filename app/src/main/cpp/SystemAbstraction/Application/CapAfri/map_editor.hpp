#pragma once

#include <library_opengles_2/RectangleRenderer/Rectangle_Renderer.hpp>
#include <library_opengles_2/RenderableObjects/SimpleObjects/GridLines.hpp>
#include <system_abstraction.hpp>
#include <library_opengles_2/TextRenderer/TextRenderer_v2.hpp>
#include "level.hpp"
#include <library_opengles_2/RectangleRenderer/LineStrip_Renderer.hpp>
#include <system_abstraction.hpp>
#include <sstream>
#include "demo.hpp"
#include "Bodies/car_body.hpp"


class Camera{
public:
    Camera()
    {
        cameraViewMatrix = glm::lookAt(glm::vec3(0,0, 10.0f),glm::vec3(0, 0, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    }

    void zoomOut()
    {
        zoom = zoom * 1.2;
        if(zoom > 50.0)
            zoom = 50.0;

        float aspect = ((float)framebuffer_width)/((float)framebuffer_height);

        box_view_width_in_meters = 30.0f*zoom;
        box_view_height_in_meters = box_view_width_in_meters/aspect;


        cameraProjectionMatrix = glm::ortho(-box_view_width_in_meters/2.0f, box_view_width_in_meters/2.0f, -box_view_height_in_meters/2.0f, box_view_height_in_meters/2.0f ,-1000.0f,1000.0f);
    }

    void zoomIn()
    {
        zoom = zoom / 1.2;
        if(zoom < 0.01)
            zoom = 0.01;

        float aspect = ((float)framebuffer_width)/((float)framebuffer_height);

        box_view_width_in_meters = 30.0f*zoom;
        box_view_height_in_meters = box_view_width_in_meters/aspect;


        cameraProjectionMatrix = glm::ortho(-box_view_width_in_meters/2.0f, box_view_width_in_meters/2.0f, -box_view_height_in_meters/2.0f, box_view_height_in_meters/2.0f ,-1000.0f,1000.0f);
    }

    GLfloat getZoom()
    {
        return zoom;
    }

    void onFrameBufferResize(int width, int height)
    {
        framebuffer_width = width;
        framebuffer_height = height;

        float aspect = ((float)framebuffer_width)/((float)framebuffer_height);

        box_view_width_in_meters = 30.0f*zoom;
        box_view_height_in_meters = box_view_width_in_meters/aspect;
        cameraProjectionMatrix = glm::ortho(-box_view_width_in_meters/2.0f, box_view_width_in_meters/2.0f, -box_view_height_in_meters/2.0f, box_view_height_in_meters/2.0f ,-1000.0f,1000.0f);

    }

    void changeXPosition(float deltaX)
    {
        camera_position_x += deltaX;
        updateCameraViewMatrix();
    }

    void changeYPosition(float deltaY)
    {
        camera_position_y += deltaY;
        updateCameraViewMatrix();
    }

    void setViewMatrix(glm::mat4 view)
    {
        cameraViewMatrix = view;
    }

    void setProjectionMatrix(glm::mat4 projection)
    {
        cameraProjectionMatrix = projection;
    }

    glm::mat4 & getViewMatrix()
    {
        return cameraViewMatrix;
    }

    glm::mat4 & getProjectionMatrix()
    {
        return cameraProjectionMatrix;
    }

    float getViewHeight()
    {
        return box_view_height_in_meters;
    }

    glm::vec3 getPosition()
    {
        return glm::vec3(camera_position_x, camera_position_y, 0.0);
    }

private:
    void updateCameraViewMatrix()
    {
        cameraViewMatrix = glm::lookAt(glm::vec3(camera_position_x, camera_position_y, 10.0f),glm::vec3(camera_position_x, camera_position_y, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    }

    glm::mat4 cameraProjectionMatrix = glm::mat4(1);
    glm::mat4 cameraViewMatrix = glm::mat4(1);

    float box_view_width_in_meters = 0.0f;
    float box_view_height_in_meters = 0.0f;

    GLfloat zoom = 1.0f;

    float framebuffer_width;
    float framebuffer_height;

    float camera_position_x = 0;
    float camera_position_y = 0;
};

class MapEditor : public MapEditorGuiEventListener{
public:

    typedef enum
    {
        CURSOR_MOVE = 0,
        CURSOR_ADD_FANT,
        CURSOR_ZOOM
    }CursorMode;

    typedef enum
    {
        FANT_GROUND = 0,
        FANT_COIN,
        FANT_MUSHROOM,
        FANT_META
    }FantMode;

    MapEditor(int win_width, int win_height);
    ~MapEditor();

    void systemCallback_WindowResize(int win_width, int win_height);
    void systemCallback_MouseScroll(double yoffset);
    void systemCallback_Render();
    void systemCallback_mouseButton(SystemAbstraction::MouseButton mouseButton, SystemAbstraction::ButtonEvent event, int x, int y);
    void systemCallback_mouseMove(int x, int y);
    void systemCallback_OnPointerMove(int pointerId, const struct PointerCoords *coords);
    void systemCallback_OnKey(SystemAbstraction::ButtonEvent event,SystemAbstraction:: Key key, SystemAbstraction::Mods mods, int x, int y);
    void systemCallback_OnChar(unsigned int codepoint);
    void systemCallback_OnPointerDown(int pointerId, const struct PointerCoords *coords) ;
    void systemCallback_OnPointerUp(int pointerId, const struct PointerCoords *coords);

    void gui_onSaveMapButtonClicked();
    void gui_onClearMapButtonClicked();
    void gui_onCursorModeChanged(int mode);
    void gui_onCurrentMapChanged(unsigned int currentMap);
    void gui_onZoomOut();
    void gui_onZoomIn();

private:
    void loadMap(string mapFilePath);

    void addGroundPointInFramebufferCoordinates(int framebuffer_x, int framebuffer_y);
    void addMetaInFramebufferCoordinates(int framebuffer_x, int framebuffer_y);
    void addCoinInFramebufferCoordinates(int framebuffer_x, int framebuffer_y);
    void addMushroomInFramebufferCoordinates(int framebuffer_x, int framebuffer_y);

    void updateCameraViewMatrix();
    void get_ndc_coordinates(float current_mouse_x_pos, float current_mouse_y_pos, float * x_ndc, float * y_ndc);
    void fbCoordToWorldCoord(double x_window, double y_window, glm::vec3 & world_position);

    PointerCoords pointer_0_coords;
    PointerCoords pointer_1_coords;


    float framebuffer_width;
    float framebuffer_height;


    CGridLines * gridLines;

    //CAR
    CarRenderer * carRenderer;

    //CAMERA
    Camera camera;

    //RED DOT
    DE_Rectangle redDotPointerRectangle;
    glm::mat4 redDotCursorModel = glm::mat4(1);
    GLuint redDotTextureId = 0;

    //LEVEL FILE
    string mapFileOpenErrorString;
    stringstream mapFilePath;
    unsigned int currentMapIndex = 0;
    int mapFileOpenErrno = 0;
    Level level;

    //TEXT RENDERER
    TextRenderer_v2 * textRenderer_v2;

    //COORDINATES LINES
    LS_LineStrip x_lineStrip;
    LS_LineStrip y_lineStrip;

    //DOTS
    vector<glm::vec3> dots;
    LS_LineStrip lineStripGround;

    FantMode fantMode = FANT_GROUND;

    CursorMode cursorMode = CURSOR_ADD_FANT;
    glm::vec3 touch_start_position_in_world;

    bool leftMouseButtonIsPressed = false;


};
