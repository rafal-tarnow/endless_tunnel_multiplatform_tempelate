#pragma once
#include <Box2D/Box2D.h>
#include <GLES2/gl2.h>
#include "./Bodies/ground_body_chain.hpp"
#include "./Bodies/background_body.hpp"
#include "./Bodies/car_body.hpp"
#include <library_opengles_2/TextRenderer/TextRenderer_v2.hpp>
#include <library_opengles_2/RectangleRenderer/Rectangle_Renderer.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "../system_abstraction.hpp"
#include "./Bodies/coin_circle_body.hpp"
#include <list>


using namespace std;

class Game : public b2ContactListener{
public:

    Game(unsigned int win_width,unsigned int win_height);
    ~Game();

    void systemCallback_WindowResize(unsigned int win_width, unsigned int win_height);
    void systemCallback_Scroll(double yoffset);
    void systemCallback_Render();
    void systemCallback_mouseButton(SystemAbstraction::MouseButton mouseButton, SystemAbstraction::ButtonEvent event, int x, int y);
    void systemCallback_keyboard(SystemAbstraction::ButtonEvent, unsigned int key, int x, int y );
    void systemCallback_TimerTick();

    //BOX2D collisions callback
    void BeginContact(b2Contact* contact);
    void PreSolve(b2Contact* contact, const b2Manifold* oldManifold);
    void PostSolve(b2Contact* contact, const b2ContactImpulse* impulse);
    void EndContact(b2Contact* contact);

private:
    void loadLevel();
    void updateGameLogics();
    b2World * getWorld();
    void renderWorldBodies();
    void renderHUD();
    void windowCoordinatesToBoxCoordinates(int x, int y, float &x_out, float &y_out);
    float current_window_width;
    float current_window_height;

    float camera_x_position_m = 1.0f;
    float camera_y_position_m = 1.5f;

    float box_view_width_in_meters = 0;
    float box_view_height_in_meters = 0;
    float box_zero_offset_meters = 0.0f;

    b2World* world;
    GroundChain * groundChain = nullptr;
    BackGround * background = nullptr;

    Car * car = nullptr;
    list<CircleCoin *> coinsList;
    vector<CircleCoin *> coinsToDelete;
    TextRenderer_v2 * textRenderer_v2 = nullptr;

    glm::mat4 projectionMatrix = glm::mat4(1);
    glm::mat4 viewMatrix = glm::mat4(1);

    GLfloat zoom = 1.0f;

    uint32_t money = 0;
};
