#pragma once

#include <library_opengles_2/RectangleRenderer/Rectangle_Renderer.hpp>
#include <library_opengles_2/RenderableObjects/SimpleObjects/GridLines.hpp>
#include <system_abstraction.hpp>
#include <library_opengles_2/TextRenderer/TextRenderer_v2.hpp>
#include "level.hpp"

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
    float camera_position_x = 0;
    float camera_position_y = 0;
    void windowCoordinatesToBoxCoordinates(double x, double y, float &x_out, float &y_out);
    double current_mouse_x_pos = 0;
    double current_mouse_y_pos = 0;
    DE_Rectangle testRectangle;
    float current_window_width;
    float current_window_height;

    float box_view_width_in_meters = 0;
    float box_view_height_in_meters = 0;

    glm::mat4 projectionMatrix = glm::mat4(1);
    glm::mat4 viewMatrix = glm::mat4(1);


    GLfloat zoom = 1.0f;

    CGridLines * gridLines;
    GLuint redDotTextureId = 0;

    string mapFileOpenErrorString;
    string mapFilePath;
    int mapFileOpenErrno = 0;
    TextRenderer_v2 * textRenderer_v2;
    Level level;
};
