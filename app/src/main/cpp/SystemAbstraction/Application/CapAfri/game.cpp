#include "game.hpp"

#include <SOIL.h>
#include <iostream>
#include <sstream>
#include <library_opengles_2/TextureManager/texture_manager.hpp>
#include "./Bodies/car_body.hpp"
#include <iomanip>
#include <library_opengles_2/TextRenderer/TextRenderer_v2.hpp>
#include "design_graffiti_agentorange_www_myfontfree_com.ttf.hpp"
#include "../../system_log.hpp"
#include <system_paths.hpp>


using namespace std;

static float angle = 0.0f;
/* render the scene */

void Game::BeginContact(b2Contact* contact)
{
    cout << "A:BeginContact(...)" << endl;
    void * userData;
    void * userData_2;

    userData = contact->GetFixtureA()->GetBody()->GetUserData();
    userData_2 = contact->GetFixtureB()->GetBody()->GetUserData();

    if(userData && userData_2)
    {
        //CHECK IS COIN COLLIDE WITH CAR
        if((((GameObject*)(userData))->getObjectType() == GameObject::OBJECT_COIN) && (((GameObject*)(userData_2))->getObjectType() == GameObject::OBJECT_CAR))
        {
            coinsToDelete.push_back((CircleCoin*)userData);
        }
        //CHECK IS COIN COLLIDE WITH CAR
        if((((GameObject*)(userData_2))->getObjectType() == GameObject::OBJECT_COIN) && (((GameObject*)(userData))->getObjectType() == GameObject::OBJECT_CAR))
        {
            coinsToDelete.push_back((CircleCoin*)userData_2);
        }
    }
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
    cout << "A:EndContact(...)" << endl;
}





Game::Game(unsigned int win_width,unsigned int win_height)
{
    current_window_width = win_width;
    current_window_height = win_height;

    //GLuint fontSize = 82;
    GLuint fontSize = GLuint(float(win_height)*0.076f);
    textRenderer_v2 = new TextRenderer_v2(current_window_width,current_window_height);
    // textRenderer_v2->Load("./data/font/design_graffiti_agentorange_www_myfontfree_com.ttf", fontSize);
    textRenderer_v2->LoadFromMemory(design_graffiti_agentorange_www_myfontfree_com_ttf, size_of_design_graffiti_agentorange_www_myfontfree_com_ttf, fontSize);

    //glEnable(GL_MULTISAMPLE);
    glEnable (GL_BLEND);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    //glClearColor(0.98,1.0,0.55,1.0);
    glClearColor(0.0,0.0,0.0,1.0);


    //glClearDepth(1.0);				// Enables Clearing Of The Depth Buffer
    //glDepthFunc(GL_LEQUAL);			// The Type Of Depth Test To Do
    //glEnable(GL_DEPTH_TEST);			// Enables Depth Testing
    //glShadeModel(GL_SMOOTH);


    world = new b2World(b2Vec2(0.0,-1.81));
    world->SetContactListener(this);

    background = new BackGround(0.0f, 10.0f, 10000.0f, 10.0f, world);
    loadLevel();

}


void Game::loadLevel()
{
    groundChain = new GroundChain(-200.0f,0.0f,10000.0f,5000.0f, 0.0f, world);

    car = new Car(1.0f, 5.0f, -1.0f, world);


    //LEVEL LOAD
    Level level;
    string mapFilePath = getAppConfigFilePath() + "/CapitanAfrica.map";
#warning "dorobic obsluge bledu otwarcia pliku"
    int mapFileOpenErrno = level.loadLevelFromFile(mapFilePath);

    //TODO dorobic delete circle coin
    for (auto & coin : level.coins_vector)
    {
        b2Vec2 pos;
        pos.x = coin->getPosition().x;
        pos.y = coin->getPosition().y;
        coins.push_back(new CircleCoin(pos.x, pos.y, -1.0f, 0.25, world));
    }
}

Game::~Game()
{

    for(unsigned int i = 0; i < coins.size(); i++ )
    {
        delete coins.at(i);
    }

    TextureManager::deleteAllTextures();
    delete background;
    delete groundChain;
    delete world;
}


b2Body* Game::addRect(int x,int y,int w,int h,bool dyn)
{
    b2BodyDef bodydef;

    bodydef.position.Set(x,y);
    if(dyn)
        bodydef.type=b2_dynamicBody;
    b2Body* body=world->CreateBody(&bodydef);

    b2PolygonShape shape;
    shape.SetAsBox(w/2,h/2);

    b2FixtureDef fixturedef;
    fixturedef.shape=&shape;
    fixturedef.density=1.0;
    body->CreateFixture(&fixturedef);
}

void Game::windowCoordinatesToBoxCoordinates(int x, int y, float &x_out, float &y_out)
{
    float x_in = (float)x;
    float y_in = (float)y;

    float szerokosc_okna_do_szerokosci_swiata = current_window_width / box_view_width_in_meters;
    float wysokosc_okna_do_wysokosci_swiata = current_window_height / box_view_height_in_meters;

    x_out = (x_in / szerokosc_okna_do_szerokosci_swiata) - camera_x_position_m;
    y_out = ((current_window_height - y_in) / wysokosc_okna_do_wysokosci_swiata) - camera_y_position_m;
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

    float aspect = ((float)current_window_width)/((float)current_window_height);

    box_view_width_in_meters = 30.0f*zoom;
    box_view_height_in_meters = box_view_width_in_meters/aspect;
    projectionMatrix = glm::ortho(-box_view_width_in_meters/2.0f, box_view_width_in_meters/2.0f, -box_view_height_in_meters/2.0f, box_view_height_in_meters/2.0f ,-1000.0f,1000.0f);
}

void Game::systemCallback_WindowResize(unsigned int win_width,unsigned int win_height)
{
    current_window_width = win_width;
    current_window_height = win_height;
    glViewport (0, 0, (GLsizei) current_window_width, (GLsizei) current_window_height);

    textRenderer_v2->onVievportResize(current_window_width, current_window_height);

    float aspect = ((float)current_window_width)/((float)current_window_height);

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
        for(unsigned int i = 0; i < coinsToDelete.size(); i++)
        {
            delete coinsToDelete.at(i);
        }
        coinsToDelete.clear();
    }
}

void Game::systemCallback_TimerTick()
{
    world->Step(1.0/30.0,8,3);
}

void Game::systemCallback_Render()
{

    updateGameLogics();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    //viewMatrix = glm::lookAt(glm::vec3(0.0f, 0.0f, 10.0f),glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));


    if(car != nullptr)
    {
        float x, y;
        car->getPosition(&x,&y);
        viewMatrix = glm::lookAt(glm::vec3(x, y, 10.0f),glm::vec3(x, y, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    }

    b2Body* tmp=world->GetBodyList();
    while(tmp)
    {
        if(tmp->GetUserData() != nullptr){
            ((GameObject *)tmp->GetUserData())->render(projectionMatrix, viewMatrix);
        }
        tmp=tmp->GetNext();
    }

    static int cash = 0;
    stringstream text;
    if(car){
        text << std::fixed << std::setprecision(0) << "DIST "<<car->getXPosition();// << " Y " << car->getYPosition();
    }else{
        text << "$ 0";
    }

    textRenderer_v2->RenderText(text.str(), current_window_width*0.03, current_window_height*0.9);

    glFlush();
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

void Game::systemCallback_keyboard(SystemAbstraction::ButtonEvent event, unsigned int key, int x, int y )
{
    if((key == 'd' || key == 'D') && (event == SystemAbstraction::EVENT_DOWN)){
        cout << "start acceleration" << endl;
        if(car) car->setSpeed(-15);
    }

    if((key == 'a' || key == 'A') && (event == SystemAbstraction::EVENT_DOWN)){
        cout << "start acceleration" << endl;

        if(car)
        {
            car->setSpeed(15);
        }
    }

    if((key == 'd' || key == 'D') && (event == SystemAbstraction::EVENT_UP)){
        cout << "stop acceleration" << endl;
        if(car) car->setSpeed(0);
    }

    if((key == 'a' || key == 'A') && (event == SystemAbstraction::EVENT_UP)){
        cout << "stop acceleration" << endl;
        if(car) car->setSpeed(0);
    }
}


