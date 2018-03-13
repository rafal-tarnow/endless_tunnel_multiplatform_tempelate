#pragma once

#include <library_opengles_2/RectangleRenderer/Rectangle_Renderer.hpp>
#include <library_opengles_2/RenderableObjects/SimpleObjects/GridLines.hpp>
#include <system_abstraction.hpp>
#include <library_opengles_2/TextRenderer/TextRenderer_v2.hpp>
#include "level.hpp"
#include <library_opengles_2/RectangleRenderer/LineStrip_Renderer.hpp>
#include <system_abstraction.hpp>
#include "demo.hpp"

class MapEditor : public GuiEventListener{
public:

    MapEditor(int win_width, int win_height);
    ~MapEditor();

    void systemCallback_WindowResize(int win_width, int win_height);
    void systemCallback_Scroll(double yoffset);
    void systemCallback_Render();
    void systemCallback_mouseButton(SystemAbstraction::MouseButton mouseButton, SystemAbstraction::ButtonEvent event, int x, int y);
    void systemCallback_OnPointerMove(int pointerId, const struct PointerCoords *coords);
    void systemCallback_OnKey(SystemAbstraction::ButtonEvent event,SystemAbstraction:: Key key, SystemAbstraction::Mods mods, int x, int y);
    void systemCallback_OnChar(unsigned int codepoint);
    void systemCallback_OnPointerDown(int pointerId, const struct PointerCoords *coords) ;
    void systemCallback_OnPointerUp(int pointerId, const struct PointerCoords *coords);

    void gui_onSaveMapButtonClicked();

private:
    void updateCameraViewMatrix();
    void get_ndc_coordinates(float current_mouse_x_pos, float current_mouse_y_pos, float * x_ndc, float * y_ndc);
    void windowCoordinatesToBoxCoordinates(double x_window, double y_window, glm::vec3 & world_position);
    double current_mouse_x_pos = 0;
    double current_mouse_y_pos = 0;

    float framebuffer_width;
    float framebuffer_height;

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
    glm::mat4 redDotCursorModel = glm::mat4(1);
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
    LS_LineStrip lineStripGround;
};
