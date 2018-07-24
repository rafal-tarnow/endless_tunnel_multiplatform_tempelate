#include "game.hpp"

#include <iostream>
#include <sstream>
#include "./Bodies/car_body.hpp"
#include <iomanip>
#include <library_opengles_2/TextRenderer/TextRenderer_v2.hpp>
#include "../../system_log.hpp"
#include <system_paths.hpp>
#include <library_opengles_2/Resources/Resources.hpp>

using namespace std;

void Game::BeginContact(b2Contact* contact)
{
    //LOGD("--> Game::BeginContact(...)");
    void * userData;
    void * userData_2;

    userData = contact->GetFixtureA()->GetBody()->GetUserData();
    userData_2 = contact->GetFixtureB()->GetBody()->GetUserData();

    if(userData && userData_2)
    {
        GameObject * object = static_cast<GameObject *>(userData);
        GameObject * object_1 = static_cast<GameObject *>(userData_2);

        //object->printObjectType();
        //object_1->printObjectType();


        //CHECK IS CAR COLLIDE WITH COIN
        if ((object->getObjectType() == GameObject::OBJECT_CAR) && (object_1->getObjectType() == GameObject::OBJECT_COIN))
        {
            if (pAudioManager)
            {
                pAudioManager->PlaySFX(m_coinHandle);
            }
            coinsToDelete.insert(static_cast<CircleCoin *>(object_1));
            money++;
            return;
        }
        //CHECK IS COIN COLLIDE WITH CAR
        if ((object->getObjectType() == GameObject::OBJECT_COIN) && (object_1->getObjectType() == GameObject::OBJECT_CAR))
        {
            if (pAudioManager)
            {
                pAudioManager->PlaySFX(m_coinHandle);
            }
            coinsToDelete.insert(static_cast<CircleCoin *>(object));
            money++;
            return;
        }

        //CHECK IS CAR COLLIDE WITH MUSHROOM
        if ((object->getObjectType() == GameObject::OBJECT_CAR) && (object_1->getObjectType() == GameObject::OBJECT_MUSHROOM))
        {
            if (pAudioManager)
            {
                pAudioManager->PlaySFX(m_mushroomHandle);
            }
            mushroomsToDelete.insert(static_cast<Mushroom *>(object_1));
            mEffects->Black = GL_TRUE;
            mushroomEffectStartTime = current_time;
            return;
        }

        //CHECK IS MUSHROOM COLLIDE WITH CAR
        if ((object->getObjectType() == GameObject::OBJECT_MUSHROOM) && (object_1->getObjectType() == GameObject::OBJECT_CAR))
        {
            if (pAudioManager)
            {
                pAudioManager->PlaySFX(m_mushroomHandle);
            }
            mushroomsToDelete.insert(static_cast<Mushroom *>(object));
            mEffects->Black = GL_TRUE;
            mushroomEffectStartTime = current_time;
            return;
        }

        //CHECK IS CAR COLLIDE WITH META
        if (((object->getObjectType() == GameObject::OBJECT_CAR) && (object_1->getObjectType() == GameObject::OBJECT_META))
                || ((object->getObjectType() == GameObject::OBJECT_META) && (object_1->getObjectType() == GameObject::OBJECT_CAR)))
        {
            if (pAudioManager)
            {
                pAudioManager->PlaySFX(m_levelCompletedHandle);
            }
            gameState = GAME_LEVEL_COMPLETED;
            return;
        }

    }

    //LOGD("<-- Game::BeginContact(...)");

}

void Game::PreSolve(b2Contact* contact, const b2Manifold* oldManifold)
{
    //cout << "A:PreSolve(...)" << endl;
}

void Game::PostSolve(b2Contact* contact, const b2ContactImpulse* impulse)
{
    //cout  << "A:PostSolve" << endl;
}

void Game::EndContact(b2Contact* contact)
{
    //cout << "A:EndContact(...)" << endl;
}



Game::Game(unsigned int fb_width,unsigned int fb_height) : camWorld(fb_width, fb_height)
{
    current_fb_width = fb_width;
    current_fb_height = fb_height;

    string configFilePath = getStandardCommonReadWriteDirecory() + "/CapitanAfrica.config";
    config.loadDataFromFileToMemory(configFilePath);

    //GLuint fontSize = 82;
    GLuint fontSize = GLuint(float(fb_height)*0.076f);
    textRenderer_v2 = new TextRenderer_v2(current_fb_width,current_fb_height);
    Resource font_design_graffiti_agentorange("fonts/design_graffiti_agentorange_www_myfontfree_com.ttf");
    textRenderer_v2->LoadFromMemory("Design graffiti agentorange", font_design_graffiti_agentorange.getData(), font_design_graffiti_agentorange.getSize(), fontSize);

    glEnable (GL_BLEND);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glClearColor(1.0,1.0,1.0,1.0);

    world = new b2World(b2Vec2(0.0,-1.81));
    world->SetContactListener(this);
    world->SetDebugDraw(&debugDraw);


    background = new BackGround(-50.0f, camWorld.getViewHeight()/2, 10000.0f, camWorld.getViewHeight(), world);

    loadLevel();
    loadAudio();
    skipBackgroundDraw =  config.get_bool("skipBackgroundDraw");
    debugDrawFlag = config.get_bool("debugDrawFlag");
    loadCoins();


    if (pAudioManager)
    {
        pAudioManager->PlaySFX(m_musicHandle);
    }

    mEffects = new PostProcessor (current_fb_width, current_fb_height);
}


void Game::loadCoins()
{
    money = config.get_uint32_t("coins");
}

void Game::saveCoins()
{
    string configFilePath = getStandardCommonReadWriteDirecory() + "/CapitanAfrica.config";
    config.set_uint32_t("coins",money);
    config.saveDataFromMemoryToFile(configFilePath);
}

void Game::loadLevel()
{
    float dampingRatio = config.get_float("dampingRatio");
    float frequencyHz = config.get_float("frequencyHz");
    float maxMotorTorque = config.get_float("maxMotorTorque");
    float friction = config.get_float("friction");
    car = new Car(glm::vec3(1.0f, 5.0f, -1.0f), world, dampingRatio, frequencyHz, maxMotorTorque, friction);


    //LEVEL LOAD
    Level level;
    uint32_t currentMapIntex = config.get_uint32_t("currentMapIndex");
    stringstream mapFilePath;
    mapFilePath << getStandardCommonReadWriteDirecory() << "/CapitanAfrica_" << currentMapIntex << ".map";
#warning "dorobic obsluge bledu otwarcia pliku"
    int mapFileOpenErrno = level.loadLevelFromFile(mapFilePath.str());

    groundChain = new GroundChain(level, -200.0f,0.0f,10000.0f,5000.0f, 0.0f, world);


    //TODO dorobic delete circle coin
    for (auto & coin : level.coins_vector)
    {
        b2Vec2 pos;
        pos.x = coin->getPosition().x;
        pos.y = coin->getPosition().y;
        coinsList.push_back(new CircleCoin(pos.x, pos.y, -1.0f, 0.25, world));
    }

    glm::vec3 metaPosition = level.meta->getPosition();
    meta = new Meta(metaPosition, 1.5, world);

    for(auto & mushroom : level.mushroom_vector)
    {
        mushroomList.push_back(new Mushroom(mushroom->getPosition(), world));
    }
}

void Game::loadAudio()
{
    AudioManager& audioManager = AudioManager::GetSingleton();
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
    delete meta;
    meta = nullptr;

    for(list<CircleCoin*>::iterator it = coinsList.begin(); it != coinsList.end(); it++)
    {
        delete (*it);
    }

    for(list<Mushroom*>::iterator it = mushroomList.begin(); it != mushroomList.end(); it++)
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
}

void Game::systemCallback_Scroll(double yoffset){
    if(yoffset > 0)
    {
        camWorld.zoomIn();
    }
    else
    {
        camWorld.zoomOut();
    }
}

void Game::systemCallback_WindowResize(unsigned int win_width,unsigned int win_height)
{
    current_fb_width = win_width;
    current_fb_height = win_height;
    glViewport (0, 0, (GLsizei) current_fb_width, (GLsizei) current_fb_height);

    textRenderer_v2->onVievportResize(current_fb_width, current_fb_height);

    camWorld.onFrameBufferResize(current_fb_width, current_fb_height);
}

void Game::updateGameLogics()
{
    if(car)
    {
        if(car->getPosition().x < 0 && (car->getSpeed() > 0))
        {
            car->setSpeed(0);
        }
    }

    
    if(coinsToDelete.size())
    {
        set<CircleCoin *>::iterator it;

        // wyświetlenie zawartości
        for( it=coinsToDelete.begin(); it != coinsToDelete.end(); it++)
        {
            coinsList.remove(*it);
            delete *it;
        }
        coinsToDelete.clear();
    }

    if(mushroomsToDelete.size())
    {
        set<Mushroom*>::iterator it;

        for(it=mushroomsToDelete.begin(); it != mushroomsToDelete.end(); it++)
        {
            mushroomList.remove(*it);
            delete *it;
        }
        mushroomsToDelete.clear();
    }

    if(mEffects->Black == GL_TRUE)
    {
        if((current_time - mushroomEffectStartTime) > 120.0)
        {
            mEffects->Black = GL_FALSE;
        }
    }
}

void Game::systemCallback_TimerTick()
{
    if(gameState == GAME_RUN)
        world->Step(1.0/30.0,8,3);

}

void Game::systemCallback_Render()
{

    //LOGD("--> Game::systemCallback_Render()\n");
    updateGameLogics();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if(gameState ==  GAME_LEVEL_COMPLETED)
    {
        mEffects->Black = GL_TRUE;
    }

    if(mEffects->Black == GL_TRUE)
        mEffects->BeginRender();
    {
        if(car != nullptr)
        {
            float x, y;
            car->getPosition(&x,&y);
            camWorld.setPosition(x,y);
        }

        if(skipBackgroundDraw == true)
        {

        }
        else
        {
            background->setModel(glm::translate(glm::mat4(1), glm::vec3(0.0, camWorld.getPosition().y, 0.0)));
            background->render(camWorld.projection(), camWorld.view());
        }

        renderWorldBodies();
        renderHUD();


    }
    if(mEffects->Black == GL_TRUE)
    {
        mEffects->EndRender();
        mEffects->Render (current_time);
    }

    if(gameState ==  GAME_LEVEL_COMPLETED)
    {
        glDisable(GL_DEPTH_TEST);
        textRenderer_v2->setColour(glm::vec4(0.0, 1.0, 0.0, 1.0f));
        textRenderer_v2->RenderText("LEVEL COMPLETED!", current_fb_width*0.5, current_fb_height*0.5, TextRenderer_v2::TEXT_CENTER);
    }

    if(debugDrawFlag == true)
    {
        debugDraw.setProjectionView(camWorld.projection(), camWorld.view());
        debugDraw.SetFlags(0xFFFFFFFF);
        world->DrawDebugData();
    }

    current_time += 0.017;
}

void Game::renderWorldBodies()
{
    if(car)
        car->render(camWorld.projection(), camWorld.view());

    if(meta)
        meta->render(camWorld.projection(), camWorld.view());

    for(list<CircleCoin*>::iterator it = coinsList.begin(); it != coinsList.end(); it++)
    {
        (*it)->render(camWorld.projection(), camWorld.view());
    }

    for(list<Mushroom*>::iterator it = mushroomList.begin(); it != mushroomList.end(); it++)
    {
        (*it)->render(camWorld.projection(), camWorld.view());
    }

    groundChain->render(camWorld.projection(), camWorld.view());
}

void Game::renderHUD()
{
    stringstream text;
    if(car){
        text << std::fixed << std::setprecision(0) << "DIST "<<car->getXPosition();
    }else{
        text << "$ 0";
    }

    textRenderer_v2->RenderText(text.str(), current_fb_width*0.03, current_fb_height*0.9);

    text.str("");
    text << "$ " << money;

    textRenderer_v2->RenderText(text.str(), current_fb_width*0.03, current_fb_height*0.8);
}

b2World * Game::getWorld()
{
    return world;
}

void Game::systemCallback_mouseButton(SystemAbstraction::MouseButton mouseButton, SystemAbstraction::ButtonEvent event, int window_x, int window_y)
{

}

void Game::systemCallback_keyboard(SystemAbstraction::ButtonEvent event, unsigned int key, int x, int y )
{
    if((key == 'd' || key == 'D') && (event == SystemAbstraction::EVENT_DOWN)){
        if(car) car->setSpeed(-15);
    }

    if((key == 'a' || key == 'A') && (event == SystemAbstraction::EVENT_DOWN)){
        if(car)
        {
            car->setSpeed(15);
        }
    }

    if((key == 'd' || key == 'D') && (event == SystemAbstraction::EVENT_UP)){
        if(car) car->setSpeed(0);
    }

    if((key == 'a' || key == 'A') && (event == SystemAbstraction::EVENT_UP)){
        if(car) car->setSpeed(0);
    }
}


