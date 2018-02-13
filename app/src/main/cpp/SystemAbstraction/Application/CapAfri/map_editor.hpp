#pragma once

#include <library_opengles_2/RectangleRenderer/Rectangle_Renderer.hpp>
#include <library_opengles_2/RenderableObjects/SimpleObjects/GridLines.hpp>
#include <system_abstraction.hpp>
#include <library_opengles_2/TextRenderer/TextRenderer_v2.hpp>
#include "level.hpp"
#include <library_opengles_2/RectangleRenderer/LineStrip_Renderer.hpp>

class MapEditor{
public:

    typedef enum{
        MOUSE_LEFT_BUTTON,
        MOUSE_MIDDLE_BUTTON,
        MOUSE_RIGHT_BUTTON,

        MOUSE_LEFT,
        MOUSE_ENTERED
    }MouseButton;


    MapEditor(int win_width, int win_height);
    ~MapEditor();

    void systemCallback_WindowResize(int win_width, int win_height);
    void systemCallback_Scroll(double yoffset);
    void systemCallback_Render();
    void systemCallback_mouseButton(MouseButton mouseButton, SystemAbstraction::ButtonEvent event, int x, int y);
    void systemCallback_mousePosition(double x, double y);
    void systemCallback_keyboard(SystemAbstraction::ButtonEvent, unsigned int key, int x, int y );

private:
    void updateCameraViewMatrix();
    void get_ndc_coordinates(float * x_ndc, float * y_ndc);
    void windowCoordinatesToBoxCoordinates(double x, double y, float &x_out, float &y_out);
    double current_mouse_x_pos = 0;
    double current_mouse_y_pos = 0;

    float current_window_width;
    float current_window_height;

    float box_view_width_in_meters = 0;
    float box_view_height_in_meters = 0;

    GLfloat zoom = 1.0f;
    CGridLines * gridLines;

    //CAMERA
    glm::mat4 cameraProjectionMatrix = glm::mat4(1);
    glm::mat4 cameraViewMatrix = glm::mat4(1);
    float camera_position_x = 0;
    float camera_position_y = 0;

    //RED DOT
    DE_Rectangle redDotPointerRectangle;
    glm::mat4 rectModel = glm::mat4(1);
    GLuint redDotTextureId = 0;

    //LEVEL FILE
    string mapFileOpenErrorString;
    string mapFilePath;
    int mapFileOpenErrno = 0;
    Level level;

    //TEXT RENDERER
    TextRenderer_v2 * textRenderer_v2;

    //COORDINATES LINES
    LS_LineStrip x_lineStrip;
    LS_LineStrip y_lineStrip;

    //DOTS
    vector<glm::vec3> dots;
};
