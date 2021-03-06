#pragma once
#include <Box2D/Box2D.h>
#include <GLES2/gl2.h>
#include "./Bodies/ground_body_chain.hpp"
#include "./Bodies/background_body.hpp"
#include "./Bodies/car_body.hpp"
#include <library_opengles_2/TextRenderer/TextRenderer_v2.hpp>
#include <library_opengles_2/RectangleRenderer/Rectangle_Renderer.hpp>
#include <library_opengles_2/RectangleRenderer/Primitive_Renderer.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "../system_abstraction.hpp"
#include "./Bodies/coin_circle_body.hpp"
#include "./Bodies/mushroom.hpp"
#include "./Bodies/meta_body.hpp"
#include <set>
#include <list>
#include <OpenSLWrap.hpp>
#include "camera.hpp"
#include "camera_safe_area.hpp"
#include "PostProcessor.hpp"
#include "debug_draw.hpp"
#include "gui/button.hpp"
#include "game_config.hpp"
#include <library_opengles_2/Debug/Debug.hpp>


using namespace std;

class Game : public b2ContactListener, public ButtonEventListener{
public:

    typedef enum{
        GAME_RUN,
        GAME_LEVEL_COMPLETED
    }GameState;

    Game(unsigned int win_width,unsigned int win_height, uint32_t levelIndex, glm::vec3 carPosition, bool mapTestMode);
    ~Game();

    void systemCallback_WindowResize(unsigned int win_width, unsigned int win_height);
    void systemCallback_Scroll(double yoffset);
    void OnPointerDown(int pointerId, const struct PointerCoords *coords);
    void OnPointerUp(int pointerId, const struct PointerCoords *coords);
    void systemCallback_Render();
    void systemCallback_mouseButton(SystemAbstraction::MouseButton mouseButton, SystemAbstraction::ButtonEvent event, int x, int y);
    void systemCallback_keyboard(SystemAbstraction::ButtonEvent, unsigned int key, int x, int y );
    void systemCallback_TimerTick();

    //BOX2D collisions callback
    void BeginContact(b2Contact* contact);
    void PreSolve(b2Contact* contact, const b2Manifold* oldManifold);
    void PostSolve(b2Contact* contact, const b2ContactImpulse* impulse);
    void EndContact(b2Contact* contact);

    void Button_onClicked(Button * button);
    void btnRetryGame();
    void btnNextLevel();

private:
    void loadCoins();
    void loadAudio();
    void loadLevel(uint32_t levelNr, glm::vec3 carPosition);
    void saveCoins();
    void updateGameLogics();
    b2World * getWorld();
    void renderWorldBodies();
    void renderHUD();
    float current_fb_width;
    float current_fb_height;

    Camera camWorld;

    SafeAreaCam safeAreaCam;

    Button buttonRetryLevel;
    Button buttonNextLevel;

    float mushroomEffectStartTime = 0.0f;
    float current_time = 0.0;


    b2World* world;
    GroundChain * groundChain = nullptr;
    BackGround * background = nullptr;

    PostProcessor* mEffects;

    Car * car = nullptr;
    Meta * meta = nullptr;
    list<CircleCoin *> coinsList;
    map<CircleCoin *,CircleCoin *> coinsToDelete;
    list<Mushroom *> mushroomList;
    map<Mushroom *, Mushroom *> mushroomsToDelete;
    TextRenderer_v2 * textRenderer_v2 = nullptr;

    uint32_t money = 0;
    bool skipBackgroundDraw = true;
    bool debugDrawFlag = false;
    glm::vec4 clearColour = glm::vec4(0.0,0.0,0.0,1.0);

    AudioManager* pAudioManager = AudioManager::GetSingletonPtr();
    AudioManager::AudioHandle m_musicHandle;
    AudioManager::AudioHandle m_coinHandle;
    AudioManager::AudioHandle m_mushroomHandle;
    AudioManager::AudioHandle m_levelCompletedHandle;

    Primitive summaryBackground;


    GameState gameState = GAME_RUN;

    GLESDebugDraw debugDraw;

    Level level;
    GameConfig * cfg = GameConfig::getInstance();

    Button buttonGaz;
    Button buttonBrake;
    SafeAreaCam safeAreaCamBreaks;

    bool useFixedSimFPS = true;
    bool useAverageSimFPS = false;
    bool useCurrentSimFPS = false;
    float simFPS = 30.0f;

    bool mapTestMode = false;

    DBG_COUNT("Game");
};
