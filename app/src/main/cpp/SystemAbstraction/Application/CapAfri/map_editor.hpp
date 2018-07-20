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
#include "camera.hpp"




class MapEditor : public MapEditorGuiEventListener{
public:

    typedef enum
    {
        CURSOR_MOVE_CAMERA = 0,
        CURSOR_ADD_FANT,
        CURSOR_ZOOM,
        CURSOR_MOVE_ELEMENT,
        CURSOR_MOVE_ELEMENT_MOVE_CAMERA,
        CURSOR_MOVE_ELEMENT_ZOOM
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
    void systemCallback_OnKey(SystemAbstraction::ButtonEvent event,SystemAbstraction:: Key key, SystemAbstraction::Mods mods, int x, int y);
    void systemCallback_OnChar(unsigned int codepoint);
    void systemCallback_OnPointerDown(int pointerId, const struct PointerCoords *coords) ;
    void systemCallback_OnPointerMove(int pointerId, const struct PointerCoords *coords);
    void systemCallback_OnPointerUp(int pointerId, const struct PointerCoords *coords);

    void gui_onSaveMapButtonClicked();
    void gui_onClearMapButtonClicked();
    void gui_onCursorModeChanged(int mode);
    void gui_onCurrentMapChanged(unsigned int currentMap);
    void gui_onZoomOut();
    void gui_onZoomIn();

private:
    void loadMap(string mapFilePath);

    void addGroundPointInFramebufferCoordinates();
    void addMetaInFramebufferCoordinates();
    void addCoinInFramebufferCoordinates();
    void addMushroomInFramebufferCoordinates();

    void updateCameraViewMatrix();
    void get_ndc_coordinates(float current_mouse_x_pos, float current_mouse_y_pos, float * x_ndc, float * y_ndc);
    void fbCoordToWorldCoord(double x_window, double y_window, glm::vec3 & world_position);
    void updateGuiDebugInfo();

    PointerCoords pointer_0_start_coords;
    PointerCoords pointer_1_start_coords;
    glm::vec3 world_start_position_0;
    glm::vec3 world_start_position_1;

    glm::vec3 world_move_camera_position_start;

    PointerCoords pointer_0_current_coords;
    PointerCoords pointer_1_current_coords;
    glm::vec3 world_current_position_0;
    glm::vec3 world_current_position_1;

    PointerCoords pointer_0_stop_coords;
    PointerCoords pointer_1_stop_coords;
    glm::vec3 world_stop_position_0;
    glm::vec3 world_stop_position_1;

    float start_zoom_distance;
    float start_camera_zoom;

    float framebuffer_width;
    float framebuffer_height;


    CGridLines * gridLines;

    //CAR
    CarRenderer * carRenderer;

    //CAMERA
    Camera camera;

    //LEVEL FILE
    string mapFileOpenErrorString;
    stringstream mapFilePath;
    unsigned int currentMapIndex = 0;
    int mapFileOpenErrno = 0;
    Level level;

    //RED DOT
    DE_Rectangle redDotPointerRectangle;
    glm::mat4 redDotCursorModel = glm::mat4(1);
    GLuint redDotTextureId = 0;

    //YELLOW DOT
    vector<glm::vec3>::iterator yellowDotIndex;
    DE_Rectangle yellowDotRectangle;
    glm::mat4 yellowDotModel = glm::mat4(1);
    GLuint yellowDotTextureId = 0;


    //TEXT RENDERER
    TextRenderer_v2 * textRenderer_v2;

    //COORDINATES LINES
    LS_LineStrip x_lineStrip;
    LS_LineStrip y_lineStrip;

    //DOTS
    LS_LineStrip lineStripGround;

    FantMode fantMode = FANT_GROUND;

    CursorMode cursorMode = CURSOR_ADD_FANT;
    CursorMode previousCursorMode = CURSOR_ADD_FANT;

    bool leftMouseButtonIsPressed = false;


};
