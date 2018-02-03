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


using namespace std;

static float angle = 0.0f;
/* render the scene */

class A: public b2ContactListener{
    void BeginContact(b2Contact* contact)
    {
        cout << "A:BeginContact(...)" << endl;
        //contact->
    }

    void EndContact(b2Contact* contact)
    {
        cout << "A:EndContact(...)" << endl;
    }

    void PreSolve(b2Contact* contact, const b2Manifold* oldManifold)
    {
        //cout << "A:PreSolve(...)" << endl;
    }

    void PostSolve(b2Contact* contact, const b2ContactImpulse* impulse)
    {
        //cout  << "A:PostSolve" << endl;
    }
};

A a;


Game::Game(unsigned int win_width,unsigned int win_height)
{
    LOGD("1.1");
    current_window_width = win_width;
    current_window_height = win_height;

    LOGD("1.2");
    DE_initShader();

    LOGD("1.3");
    //GLuint fontSize = 82;
    GLuint fontSize = GLuint(float(win_height)*0.076f);
    textRenderer_v2 = new TextRenderer_v2(current_window_width,current_window_height);
   // textRenderer_v2->Load("./data/font/design_graffiti_agentorange_www_myfontfree_com.ttf", fontSize);
    textRenderer_v2->LoadFromMemory(design_graffiti_agentorange_www_myfontfree_com_ttf, size_of_design_graffiti_agentorange_www_myfontfree_com_ttf, fontSize);
    LOGD("1.4");

    //glEnable(GL_MULTISAMPLE);
    glEnable (GL_BLEND);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glClearColor(0.98,1.0,0.55,1.0);

    LOGD("1.5");

    //glClearDepth(1.0);				// Enables Clearing Of The Depth Buffer
    //glDepthFunc(GL_LEQUAL);			// The Type Of Depth Test To Do
    //glEnable(GL_DEPTH_TEST);			// Enables Depth Testing
    //glShadeModel(GL_SMOOTH);


    LOGD("1.6");
    world = new b2World(b2Vec2(0.0,-1.81));
    LOGD("1.7");
    world->SetContactListener(&a);
    LOGD("1.8");
    background = new BackGround(0.0f, 10.0f, 10000.0f, 10.0f, world);
    LOGD("1.9");
    loadLevel();

    LOGD("1.10");
    GLuint tex;
//      glGenTextures(1, &tex);
//      glBindTexture(GL_TEXTURE_2D, tex);

  //    float pixels[] = {
  //            0.0f, 0.0f, 0.0f,   1.0f, 1.0f, 1.0f,
  //            1.0f, 1.0f, 1.0f,   0.0f, 0.0f, 0.0f
  //    };

//      unsigned char pixels[] = {
//              255, 255, 255, 255,   255, 255, 255, 255,
//              255, 255, 255, 255,   255, 255, 255, 255
//      };

      unsigned char pixels[] = {
              255, 255, 255, 255};

//      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 2, 2, 0, GL_RGBA, GL_FLOAT, pixels);
//      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
//      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

//      glBindTexture(GL_TEXTURE_2D, 0);

   //  tex = SOIL_load_OGL_texture("./data/png/coin_2.png", 4,0,SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y);



    LOGD("1.11");


      DE_initRectangle_1(&rectangle, tex, -1, 1, 1, -1, 0);
    LOGD("1.12");
}


void Game::loadLevel()
{
    LOGD("1.9.1");
    ground = new Ground(-200.0f,0.0f,10000.0f,5000.0f, 0.0f, world);
    LOGD("1.9.2");
    car = new Car(1.0f, 5.0f, world);
    LOGD("1.9.3");
    float x_coin_position = 10;
    LOGD("1.9.4");
    for(int i = 0; i < 100; i++)
    {
        coin = new CircleCoin(x_coin_position,0.75,0.25, world);
        x_coin_position += 5.0f;
    }
    LOGD("1.9.5");
}

Game::~Game()
{
    TextureManager::deleteAllTextures();
    delete background;
    delete ground;
    delete world;
     DE_deleteRectangle(&rectangle);
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
            ((RenderableObject *)tmp->GetUserData())->render(projectionMatrix, viewMatrix);
        }
        tmp=tmp->GetNext();
    }



    static int cash = 0;
    stringstream text;
    if(car){
        text << std::fixed << std::setprecision(0) << "$ "<<car->getXPosition();
    }else{
        text << "$ 0";
    }

    textRenderer_v2->RenderText(text.str(), current_window_width*0.03, current_window_height*0.9);

    rectangle.projection = glm::mat4(1);
    rectangle.view = glm::mat4(1);
    rectangle.model = glm::mat4(1);


    //DE_drawRectangle(&rectangle);

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


