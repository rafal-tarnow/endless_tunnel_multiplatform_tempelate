#include "game.hpp"

#include "../../system_log.hpp"
#include "../Tunnel/play_capafri_scene.hpp"
#include "../Tunnel/scene_manager.hpp"
#include "../Tunnel/scene_select_map.hpp"
#include "./Bodies/car_body.hpp"
#include <iomanip>
#include <iostream>
#include <library_opengles_2/Resources/Resources.hpp>
#include <library_opengles_2/TextRenderer/TextRenderer_v2.hpp>
#include <library_opengles_2/TextureManager/texture_manager.hpp>
#include <sstream>
#include <system_paths.hpp>

using namespace std;

void Game::BeginContact(b2Contact *contact)
{
    // LOGD("--> Game::BeginContact(...)");
    void *userData;
    void *userData_2;

    userData = contact->GetFixtureA()->GetBody()->GetUserData();
    userData_2 = contact->GetFixtureB()->GetBody()->GetUserData();

    if (userData && userData_2)
    {
        GameObject *object = static_cast<GameObject *>(userData);
        GameObject *object_1 = static_cast<GameObject *>(userData_2);

        // object->printObjectType();
        // object_1->printObjectType();

        // CHECK IS CAR COLLIDE WITH COIN
        if ((object->getObjectType() == GameObject::OBJECT_CAR) && (object_1->getObjectType() == GameObject::OBJECT_COIN))
        {
            if (pAudioManager)
            {
                pAudioManager->PlaySFX(m_coinHandle);
            }
            CircleCoin *circleCoin = static_cast<CircleCoin *>(object_1);
            coinsToDelete[circleCoin] = circleCoin;
            money++;
            return;
        }
        // CHECK IS COIN COLLIDE WITH CAR
        if ((object->getObjectType() == GameObject::OBJECT_COIN) && (object_1->getObjectType() == GameObject::OBJECT_CAR))
        {
            if (pAudioManager)
            {
                pAudioManager->PlaySFX(m_coinHandle);
            }
            CircleCoin *circleCoin = static_cast<CircleCoin *>(object);
            coinsToDelete[circleCoin] = circleCoin;
            money++;
            return;
        }

        // CHECK IS CAR COLLIDE WITH MUSHROOM
        if ((object->getObjectType() == GameObject::OBJECT_CAR) && (object_1->getObjectType() == GameObject::OBJECT_MUSHROOM))
        {
            if (pAudioManager)
            {
                pAudioManager->PlaySFX(m_mushroomHandle);
            }
            Mushroom *mushroom = static_cast<Mushroom *>(object_1);
            mushroomsToDelete[mushroom] = mushroom;
            mEffects->Black = GL_TRUE;
            mushroomEffectStartTime = current_time;
            return;
        }

        // CHECK IS MUSHROOM COLLIDE WITH CAR
        if ((object->getObjectType() == GameObject::OBJECT_MUSHROOM) && (object_1->getObjectType() == GameObject::OBJECT_CAR))
        {
            if (pAudioManager)
            {
                pAudioManager->PlaySFX(m_mushroomHandle);
            }
            Mushroom *mushroom = static_cast<Mushroom *>(object);
            mushroomsToDelete[mushroom] = mushroom;
            mEffects->Black = GL_TRUE;
            mushroomEffectStartTime = current_time;
            return;
        }

        // CHECK IS CAR COLLIDE WITH META
        if (((object->getObjectType() == GameObject::OBJECT_CAR) && (object_1->getObjectType() == GameObject::OBJECT_META)) ||
            ((object->getObjectType() == GameObject::OBJECT_META) && (object_1->getObjectType() == GameObject::OBJECT_CAR)))
        {
            if (pAudioManager)
            {
                pAudioManager->PlaySFX(m_levelCompletedHandle);
            }
            gameState = GAME_LEVEL_COMPLETED;
            return;
        }
    }

    // LOGD("<-- Game::BeginContact(...)");
}

void Game::PreSolve(b2Contact *contact, const b2Manifold *oldManifold)
{
    // cout << "A:PreSolve(...)" << endl;
}

void Game::PostSolve(b2Contact *contact, const b2ContactImpulse *impulse)
{
    // cout  << "A:PostSolve" << endl;
}

void Game::EndContact(b2Contact *contact)
{
    // cout << "A:EndContact(...)" << endl;
}

void Game::btnRetryGame()
{
    cout << "retryGame()" << endl;
    SceneManager::GetInstance()->RequestNewScene(new PlayCapAfriScene());
}
void Game::btnNextLevel()
{
    if (cfg->currentMapIndex < 2)
        cfg->currentMapIndex++;

    SceneManager::GetInstance()->RequestNewScene(new SelectMapScene());
    cout << "nextLevel()" << endl;
}

void Game::Button_onClicked(Button *button)
{
    if (button == &buttonGaz)
    {
        cout << "Button Gaz clicked" << endl;
    }
    else if (button == &buttonBrake)
    {
        cout << "Button Brake clicked" << endl;
    }
    else if (button == &buttonRetryLevel)
    {
        cout << "Button Retry clicked" << endl;
        btnRetryGame();
    }
    else if (button == &buttonNextLevel)
    {
        cout << "Button Next clicked" << endl;
        btnNextLevel();
    }
}

Game::Game(unsigned int fb_width, unsigned int fb_height, uint32_t levelIndex, glm::vec3 carPosition, bool testMode) : camWorld(fb_width, fb_height)
{

    mapTestMode = testMode;

    current_fb_width = fb_width;
    current_fb_height = fb_height;

    // GLuint fontSize = 82;
    GLuint fontSize = GLuint(float(fb_height) * 0.076f);

    textRenderer_v2 = new TextRenderer_v2(current_fb_width, current_fb_height);
    Resource font_design_graffiti_agentorange("fonts/design_graffiti_agentorange_www_myfontfree_com.ttf");
    textRenderer_v2->LoadFromMemory("design_graffiti_agentorange_www_myfontfree_com.ttf", font_design_graffiti_agentorange.getData(), font_design_graffiti_agentorange.getSize(), fontSize);

    world = new b2World(b2Vec2(0.0, -1.81));
    LOGD("Game::Game(9)");
    world->SetContactListener(this);
    LOGD("Game::Game(10)");
    world->SetDebugDraw(&debugDraw);
    LOGD("Game::Game(11)");

    loadLevel(levelIndex, carPosition);
    LOGD("Game::Game(12)");
    loadAudio();
    LOGD("Game::Game(13)");
    skipBackgroundDraw = cfg->skipBackgroundDraw;
    LOGD("Game::Game(14)");
    debugDrawFlag = cfg->debugDrawFlag;
    LOGD("Game::Game(15)");
    clearColour = cfg->clearColour;
    LOGD("Game::Game(16)");
    loadCoins();
    LOGD("Game::Game(17)");

    glEnable(GL_BLEND);
    LOGD("Game::Game(18)");
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    LOGD("Game::Game(19)");
    glClearColor(clearColour.r, clearColour.g, clearColour.b, clearColour.a);
    LOGD("Game::Game(20)");

    if (pAudioManager)
    {
        pAudioManager->PlaySFX(m_musicHandle);
    }
    LOGD("Game::Game(21)");
    mEffects = new PostProcessor(current_fb_width, current_fb_height);
    LOGD("Game::Game(22)");

    safeAreaCam.setSafeAreaDim(glm::vec2(1.0, 1.0));
    LOGD("Game::Game(23)");
    safeAreaCam.onResize(current_fb_width, current_fb_height);
    LOGD("Game::Game(24)");

    glm::vec3 vert[4];
    LOGD("Game::Game(25)");
    vert[0] = glm::vec3(0.0, 1.0, 2.0);
    vert[1] = glm::vec3(0.0, 0.0, 2.0);
    vert[2] = glm::vec3(1.0, 0.0, 2.0);
    vert[3] = glm::vec3(1.0, 1.0, 2.0);
    //    vert[0] = glm::vec3(0.25,0.75,2.0);
    //    vert[1] = glm::vec3(0.25,0.25,2.0);
    //    vert[2] = glm::vec3(0.75,0.25,2.0);
    //    vert[3] = glm::vec3(0.75,0.75,2.0);
    LOGD("Game::Game(26)");
    PR_init(&summaryBackground, &vert[0], 4, glm::vec4(0.0, 0.0, 0.0, 0.72), GL_TRIANGLE_FAN, GL_STATIC_DRAW);
    LOGD("Game::Game(27)");

    glm::vec3 position = glm::vec3((1.0f / 3.0f), (1.0f / 4.0f), 0.0f);
    buttonRetryLevel.setPosition(position);
    buttonRetryLevel.setDimm(glm::vec2(0.2, 0.2));
    buttonRetryLevel.setMatrices(&safeAreaCam.viewport(), &safeAreaCam.projection(), &safeAreaCam.view());
    buttonRetryLevel.setNormalBackgroundTexture(TextureManager::getInstance()->getTextureId("textures/replay_white.png"));
    buttonRetryLevel.setPressedBackgroundTexture(TextureManager::getInstance()->getTextureId("textures/replay_white.png"));
    buttonRetryLevel.setEventListener(this);

    LOGD("Game::Game(28)");
    position = glm::vec3((2.0f / 3.0f), (1.0f / 4.0f), 0.0f);
    buttonNextLevel.setPosition(position);
    buttonNextLevel.setDimm(glm::vec2(0.2, 0.2));
    buttonNextLevel.setMatrices(&safeAreaCam.viewport(), &safeAreaCam.projection(), &safeAreaCam.view());
    buttonNextLevel.setNormalBackgroundTexture(TextureManager::getInstance()->getTextureId("textures/continue_white.png"));
    buttonNextLevel.setPressedBackgroundTexture(TextureManager::getInstance()->getTextureId("textures/continue_white.png"));
    buttonNextLevel.setEventListener(this);
    LOGD("Game::Game(29)");

    glm::vec2 safe_area_dimension = glm::vec2(1920.0f, 1080.0f);
    safeAreaCamBreaks.setSafeAreaDim(safe_area_dimension);
    safeAreaCamBreaks.onResize(current_fb_width, current_fb_height);

    position = glm::vec3(1920.0f - 265.0f, 0.0f + 128.0, 0.0f);
    buttonGaz.setPosition(position);
    buttonGaz.setDimm(glm::vec2(256, 256));
    buttonGaz.setMatrices(&safeAreaCamBreaks.viewport(), &safeAreaCamBreaks.projection(), &safeAreaCamBreaks.view());
    buttonGaz.setNormalBackgroundTexture(TextureManager::getInstance()->getTextureId("textures/gaz.png"));
    buttonGaz.setPressedBackgroundTexture(TextureManager::getInstance()->getTextureId("textures/gaz_pressed.png"));
    buttonGaz.setEventListener(this);

    position = glm::vec3(256.0f, 0.0f + 128.0, 0.0f);
    buttonBrake.setPosition(position);
    buttonBrake.setDimm(glm::vec2(256, 256));
    buttonBrake.setMatrices(&safeAreaCamBreaks.viewport(), &safeAreaCamBreaks.projection(), &safeAreaCamBreaks.view());
    buttonBrake.setNormalBackgroundTexture(TextureManager::getInstance()->getTextureId("textures/brake.png"));
    buttonBrake.setPressedBackgroundTexture(TextureManager::getInstance()->getTextureId("textures/brake_pressed.png"));
    buttonBrake.setEventListener(this);
}

void Game::loadCoins()
{
    money = cfg->coins;

    useFixedSimFPS = cfg->useFixedSimFPS;
    useAverageSimFPS = cfg->useAverageSimFPS;
    useCurrentSimFPS = cfg->useCurrentSimFPS;
    simFPS = cfg->simFPS;
}

void Game::saveCoins()
{
    string configFilePath = getStandardCommonReadWriteDirecory() + "/CapitanAfrica.config";
    cfg->coins = money;
    cfg->useFixedSimFPS = useFixedSimFPS;
    cfg->useAverageSimFPS = useAverageSimFPS;
    cfg->useCurrentSimFPS = useCurrentSimFPS;
    cfg->simFPS = simFPS;
}

void Game::loadLevel(uint32_t levelNr, glm::vec3 carPosition)
{
    float dampingRatio = cfg->dampingRatio;
    float frequencyHz = cfg->frequencyHz;
    float maxMotorTorque = cfg->maxMotorTorque;
    float friction = cfg->friction;
    car = new Car(carPosition, world, dampingRatio, frequencyHz, maxMotorTorque, friction);

    // LEVEL LOAD
    stringstream mapFilePath;
    mapFilePath << "/CapitanAfrica_" << levelNr << ".map";
#warning "dorobic obsluge bledu otwarcia pliku"
    int mapFileOpenErrno = level.loadLevelFromFile(getStandardCommonReadWriteDirecory() + mapFilePath.str());

    // Resource mapFromAssets("maps" + mapFilePath.str());
    // level.loadLevelFromMemory(mapFromAssets.getData(), mapFromAssets.getSize());

    groundChain = new GroundChain(level, -200.0f, 0.0f, 10000.0f, 5000.0f, 0.0f, world);

    // TODO dorobic delete circle coin
    for (auto &coin : level.coins_vector)
    {
        b2Vec2 pos;
        pos.x = coin->getPosition().x;
        pos.y = coin->getPosition().y;
        coinsList.push_back(new CircleCoin(pos.x, pos.y, -1.0f, 0.25, world));
    }

    glm::vec3 metaPosition = level.meta->getPosition();
    meta = new Meta(metaPosition, 1.5, world);

    for (auto &mushroom : level.mushroom_vector)
    {
        mushroomList.push_back(new Mushroom(mushroom->getPosition(), world));
    }

    background = new BackGround(-50.0f, camWorld.getViewHeight() / 2, 10000.0f, camWorld.getViewHeight(), level.background_image_index);
}

void Game::loadAudio()
{
    AudioManager &audioManager = AudioManager::GetSingleton();
    std::string musicName("sounds/music_game.ogg");
    m_musicHandle = audioManager.CreateSFX(musicName, true);

    std::string coinEffectName("sounds/coin.wav");
    m_coinHandle = audioManager.CreateSFX(coinEffectName, false);

    std::string mushroomEffectName("sounds/mushroom.wav");
    m_mushroomHandle = audioManager.CreateSFX(mushroomEffectName, false);

    std::string levelCompletedEffectName("sounds/level_up.wav");
    m_levelCompletedHandle = audioManager.CreateSFX(levelCompletedEffectName, false);
}

Game::~Game()
{
    PR_delete(&summaryBackground);
    delete meta;
    meta = nullptr;

    for (list<CircleCoin *>::iterator it = coinsList.begin(); it != coinsList.end(); it++)
    {
        delete (*it);
    }

    for (list<Mushroom *>::iterator it = mushroomList.begin(); it != mushroomList.end(); it++)
    {
        delete (*it);
    }

    AudioManager::GetSingleton().StopSFX(m_musicHandle);

    delete background;
    delete groundChain;
    delete car;
    delete world;
    delete textRenderer_v2;

    saveCoins();

    if (mEffects)
    {
        delete mEffects;
        mEffects = nullptr;
    }

    cfg->sync();
}

void Game::systemCallback_Scroll(double yoffset)
{
    if (yoffset > 0)
    {
        camWorld.zoomIn();
    }
    else
    {
        camWorld.zoomOut();
    }
}

void Game::systemCallback_WindowResize(unsigned int win_width, unsigned int win_height)
{
    current_fb_width = win_width;
    current_fb_height = win_height;
    glViewport(0, 0, (GLsizei)current_fb_width, (GLsizei)current_fb_height);

    textRenderer_v2->onVievportResize(current_fb_width, current_fb_height);

    camWorld.onFrameBufferResize(current_fb_width, current_fb_height);

    safeAreaCam.onResize(current_fb_width, current_fb_height);
    safeAreaCamBreaks.onResize(current_fb_width, current_fb_height);
}

void Game::updateGameLogics()
{
    if (car)
    {
        if (car->getPosition().x < 0 && (car->getSpeed() > 0))
        {
            car->setSpeed(0);
        }
    }

    if (coinsToDelete.size())
    {
        map<CircleCoin *, CircleCoin *>::iterator it;

        // wyświetlenie zawartości
        for (it = coinsToDelete.begin(); it != coinsToDelete.end(); it++)
        {
            coinsList.remove((*it).second);
            delete (*it).second;
        }
        coinsToDelete.clear();
    }

    if (mushroomsToDelete.size())
    {
        map<Mushroom *, Mushroom *>::iterator it;

        for (it = mushroomsToDelete.begin(); it != mushroomsToDelete.end(); it++)
        {
            mushroomList.remove((*it).second);
            delete (*it).second;
        }
        mushroomsToDelete.clear();
    }

    if (mEffects->Black == GL_TRUE)
    {
        if ((current_time - mushroomEffectStartTime) > 120.0)
        {
            mEffects->Black = GL_FALSE;
        }
    }
}

void Game::systemCallback_TimerTick()
{
    if (gameState == GAME_RUN)
    {
        if (useFixedSimFPS)
            world->Step(1.0 / simFPS, 8, 3);
        else if (useCurrentSimFPS)
            world->Step(1.0 / (float(current_fps) * 0.5f), 8, 3);
        else if (useAverageSimFPS)
            world->Step(1.0 / (float(average_fps) * 0.5f), 8, 3);
    }
}

void Game::OnPointerDown(int pointerId, const struct PointerCoords *coords)
{
    // cout << "Game::OnPointerDown" << endl;
    if (gameState == GAME_LEVEL_COMPLETED)
    {
        buttonRetryLevel.onPointerDown(coords->x, coords->y);
        buttonNextLevel.onPointerDown(coords->x, coords->y);
    }
    else
    {
        buttonGaz.onPointerDown(coords->x, coords->y);
        buttonBrake.onPointerDown(coords->x, coords->y);
    }
}

void Game::OnPointerUp(int pointerId, const struct PointerCoords *coords)
{
    buttonRetryLevel.onPointerUp();
    buttonNextLevel.onPointerUp();

    buttonGaz.onPointerUp();
    buttonBrake.onPointerUp();
}

void Game::systemCallback_Render()
{

    // LOGD("--> Game::systemCallback_Render()\n");
    updateGameLogics();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if (gameState == GAME_LEVEL_COMPLETED)
    {
        mEffects->Black = GL_TRUE;
    }

    if (mEffects->Black == GL_TRUE)
        mEffects->BeginRender();
    {
        if (car != nullptr)
        {
            float x, y;
            car->getPosition(&x, &y);
            camWorld.setPosition(x + level.cameraOffset.x, y + level.cameraOffset.y);
        }

        if (skipBackgroundDraw == true)
        {
        }
        else
        {
            background->setModel(glm::translate(glm::mat4(1), glm::vec3(0.0, camWorld.getPosition().y, 0.0)));
            background->render(camWorld.projection(), camWorld.view());
        }

        renderWorldBodies();
        renderHUD();

        buttonGaz.Render();
        buttonBrake.Render();
    }
    if (mEffects->Black == GL_TRUE)
    {
        mEffects->EndRender();
        mEffects->Render(current_time);
    }

    if (gameState == GAME_LEVEL_COMPLETED)
    {

        summaryBackground.projection = safeAreaCam.projection();
        summaryBackground.view = safeAreaCam.view();
        summaryBackground.model = glm::mat4(1);
        PR_draw(&summaryBackground, 1.0);

        glDisable(GL_DEPTH_TEST);
        textRenderer_v2->setColour(glm::vec4(0.0, 1.0, 0.0, 1.0f));
        textRenderer_v2->RenderText(3, "LEVEL COMPLETED!", current_fb_width * 0.5, current_fb_height * 0.5, TextRenderer_v2::TEXT_CENTER);

        buttonRetryLevel.Render();
        buttonNextLevel.Render();
    }

    if (debugDrawFlag == true)
    {
        debugDraw.setProjectionView(camWorld.projection(), camWorld.view());
        debugDraw.SetFlags(0xFFFFFFFF);
        world->DrawDebugData();
    }

    current_time += 0.017;
}

void Game::renderWorldBodies()
{
    if (car)
        car->render(camWorld.projection(), camWorld.view());

    if (meta)
        meta->render(camWorld.projection(), camWorld.view());

    for (list<CircleCoin *>::iterator it = coinsList.begin(); it != coinsList.end(); it++)
    {
        (*it)->render(camWorld.projection(), camWorld.view());
    }

    for (list<Mushroom *>::iterator it = mushroomList.begin(); it != mushroomList.end(); it++)
    {
        (*it)->render(camWorld.projection(), camWorld.view());
    }

    groundChain->render(camWorld.projection(), camWorld.view());
}

void Game::renderHUD()
{
    stringstream text;
    if (car)
    {
        text << std::fixed << std::setprecision(0) << "DIST " << car->getXPosition();
    }
    else
    {
        text << "$ 0";
    }

    textRenderer_v2->RenderText(1, text.str(), current_fb_width * 0.03, current_fb_height * 0.9);

    text.str("");
    text << "$ " << money;

    textRenderer_v2->RenderText(2, text.str(), current_fb_width * 0.03, current_fb_height * 0.8);

}

b2World *Game::getWorld()
{
    return world;
}

void Game::systemCallback_mouseButton(SystemAbstraction::MouseButton mouseButton, SystemAbstraction::ButtonEvent event, int window_x, int window_y)
{
}

void Game::systemCallback_keyboard(SystemAbstraction::ButtonEvent event, unsigned int key, int x, int y)
{
    if ((key == 'd' || key == 'D') && (event == SystemAbstraction::EVENT_DOWN))
    {
        if (car)
        {
            car->setSpeed(-15);
            if (gameState == GAME_RUN)
                buttonGaz.setPressed(true);
        }
    }

    if ((key == 'd' || key == 'D') && (event == SystemAbstraction::EVENT_UP))
    {
        if (car)
        {
            car->setSpeed(0);
            buttonGaz.setPressed(false);
        }
    }

    if ((key == 'a' || key == 'A') && (event == SystemAbstraction::EVENT_DOWN))
    {
        if (car)
        {
            car->setSpeed(15);
            if (gameState == GAME_RUN)
                buttonBrake.setPressed(true);
        }
    }

    if ((key == 'a' || key == 'A') && (event == SystemAbstraction::EVENT_UP))
    {
        if (car)
            car->setSpeed(0);
        buttonBrake.setPressed(false);
    }
}
