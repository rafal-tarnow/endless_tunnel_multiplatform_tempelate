#include "game.hpp"

#include <iostream>
#include <sstream>
#include <library_opengles_2/TextureManager/texture_manager.hpp>
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
            AudioManager* pAudioManager = AudioManager::GetSingletonPtr();
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
            AudioManager* pAudioManager = AudioManager::GetSingletonPtr();
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
            AudioManager* pAudioManager = AudioManager::GetSingletonPtr();
            if (pAudioManager)
            {
                pAudioManager->PlaySFX(m_mushroomHandle);
            }
            mushroomsToDelete.insert(static_cast<Mushroom *>(object_1));
            mEffects->Spin = GL_TRUE;
            mushroomEffectStartTime = current_time;
            return;
        }

        //CHECK IS MUSHROOM COLLIDE WITH CAR
        if ((object->getObjectType() == GameObject::OBJECT_MUSHROOM) && (object_1->getObjectType() == GameObject::OBJECT_CAR))
        {
            AudioManager* pAudioManager = AudioManager::GetSingletonPtr();
            if (pAudioManager)
            {
                pAudioManager->PlaySFX(m_mushroomHandle);
            }
            mushroomsToDelete.insert(static_cast<Mushroom *>(object));
            mEffects->Spin = GL_TRUE;
            mushroomEffectStartTime = current_time;
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



Game::Game(unsigned int fb_width,unsigned int fb_height)
{
    current_fb_width = fb_width;
    current_fb_height = fb_height;

    string configFilePath = getAppConfigFilePath() + "/CapitanAfrica.config";
    config.loadDataFromFileToMemory(configFilePath);

    //GLuint fontSize = 82;
    GLuint fontSize = GLuint(float(fb_height)*0.076f);
    textRenderer_v2 = new TextRenderer_v2(current_fb_width,current_fb_height);
    Resource font_design_graffiti_agentorange("fonts/design_graffiti_agentorange_www_myfontfree_com.ttf");
    textRenderer_v2->LoadFromMemory(font_design_graffiti_agentorange.getData(), font_design_graffiti_agentorange.getSize(), fontSize);

    glEnable (GL_BLEND);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glClearColor(0.0,0.0,0.0,1.0);

    world = new b2World(b2Vec2(0.0,-1.81));
    world->SetContactListener(this);

    background = new BackGround(0.0f, 10.0f, 10000.0f, 10.0f, world);

    loadLevel();
    loadAudio();
    loadCoins();

    AudioManager* pAudioManager = AudioManager::GetSingletonPtr();
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
    string configFilePath = getAppConfigFilePath() + "/CapitanAfrica.config";
    config.set_uint32_t("coins",money);
    config.saveDataFromMemoryToFile(configFilePath);
}

void Game::loadLevel()
{


    float dampingRatio = config.get_float("dampingRatio");
    float frequencyHz = config.get_float("frequencyHz");
    float maxMotorTorque = config.get_float("maxMotorTorque");
    float friction = config.get_float("friction");
    car = new Car(1.0f, 5.0f, -1.0f, world, dampingRatio, frequencyHz, maxMotorTorque, friction);


    //LEVEL LOAD
    Level level;
    uint32_t currentMapIntex = config.get_uint32_t("currentMapIndex");
    stringstream mapFilePath;
    mapFilePath << getAppConfigFilePath() << "/CapitanAfrica_" << currentMapIntex << ".map";
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
}

Game::~Game()
{
    for(list<CircleCoin*>::iterator it = coinsList.begin(); it != coinsList.end(); it++)
    {
        delete (*it);
    }

    for(list<Mushroom*>::iterator it = mushroomList.begin(); it != mushroomList.end(); it++)
    {
        delete (*it);
    }

    AudioManager::GetSingleton().StopSFX(m_musicHandle);

    TextureManager::deleteAllTextures();
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

void Game::windowCoordinatesToBoxCoordinates(int x, int y, float &x_out, float &y_out)
{
    float x_in = (float)x;
    float y_in = (float)y;

    float szerokosc_okna_do_szerokosci_swiata = current_fb_width / box_view_width_in_meters;
    float wysokosc_okna_do_wysokosci_swiata = current_fb_height / box_view_height_in_meters;

    x_out = (x_in / szerokosc_okna_do_szerokosci_swiata) - camera_x_position_m;
    y_out = ((current_fb_height - y_in) / wysokosc_okna_do_wysokosci_swiata) - camera_y_position_m;
}

void Game::systemCallback_Scroll(double yoffset){
    if(yoffset > 0)
    {
        zoom = zoom / 1.2;
        if(zoom < 0.2)
            zoom = 0.2;
    }
    else
    {
        zoom = zoom * 1.2;
        if(zoom > 5)
            zoom = 5.0;
    }

    float aspect = ((float)current_fb_width)/((float)current_fb_height);

    box_view_width_in_meters = 30.0f*zoom;
    box_view_height_in_meters = box_view_width_in_meters/aspect;
    projectionMatrix = glm::ortho(-box_view_width_in_meters/2.0f, box_view_width_in_meters/2.0f, -box_view_height_in_meters/2.0f, box_view_height_in_meters/2.0f ,-1000.0f,1000.0f);
}

void Game::systemCallback_WindowResize(unsigned int win_width,unsigned int win_height)
{
    current_fb_width = win_width;
    current_fb_height = win_height;
    glViewport (0, 0, (GLsizei) current_fb_width, (GLsizei) current_fb_height);

    textRenderer_v2->onVievportResize(current_fb_width, current_fb_height);

    float aspect = ((float)current_fb_width)/((float)current_fb_height);

    box_view_width_in_meters = 18.0f*zoom;
    box_view_height_in_meters = box_view_width_in_meters/aspect;
    projectionMatrix = glm::ortho(-box_view_width_in_meters/2.0f, box_view_width_in_meters/2.0f, -box_view_height_in_meters/2.0f, box_view_height_in_meters/2.0f ,-1000.0f,1000.0f);
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

    if(mEffects->Spin == GL_TRUE)
    {
        if((current_time - mushroomEffectStartTime) > 12.0)
        {
            mEffects->Spin = GL_FALSE;
        }
    }
}

void Game::systemCallback_TimerTick()
{
    world->Step(1.0/30.0,8,3);
}

void Game::systemCallback_Render()
{

    //LOGD("--> Game::systemCallback_Render()\n");
    updateGameLogics();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if(mEffects->Spin == GL_TRUE)
        mEffects->BeginRender();
    {
        if(car != nullptr)
        {
            float x, y;
            car->getPosition(&x,&y);
            viewMatrix = glm::lookAt(glm::vec3(x, y, 10.0f),glm::vec3(x, y, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        }

        renderWorldBodies();
        renderHUD();
    }
    if(mEffects->Spin == GL_TRUE)
    {
        mEffects->EndRender();
        mEffects->Render (current_time);
    }



    glFlush();
    //LOGD("<-- Game::systemCallback_Render()\n");

    current_time += 0.030;
}

void Game::renderWorldBodies()
{
    if(car)
        car->render(projectionMatrix, viewMatrix);

    for(list<CircleCoin*>::iterator it = coinsList.begin(); it != coinsList.end(); it++)
    {
        (*it)->render(projectionMatrix, viewMatrix);
    }

    for(list<Mushroom*>::iterator it = mushroomList.begin(); it != mushroomList.end(); it++)
    {
        (*it)->render(projectionMatrix, viewMatrix);
    }
    groundChain->render(projectionMatrix, viewMatrix);
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
    float world_x, world_y;
    windowCoordinatesToBoxCoordinates(window_x, window_y, world_x, world_y);

    if ((mouseButton == SystemAbstraction::MOUSE_LEFT_BUTTON) && (event == SystemAbstraction::EVENT_DOWN))
    {


    }else if((mouseButton == SystemAbstraction::MOUSE_RIGHT_BUTTON) && (event == SystemAbstraction::EVENT_DOWN))
    {

    }
}

#include <system_audio.hpp>

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


