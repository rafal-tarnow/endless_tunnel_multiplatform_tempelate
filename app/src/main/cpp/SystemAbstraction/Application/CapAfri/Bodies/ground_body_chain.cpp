#include "ground_body_chain.hpp"

#include <iostream>
#include <library_opengles_2/TextureManager/texture_manager.hpp>
#include <system_paths.hpp>

using namespace std;


GroundChain::GroundChain(float x_top_left, float y_top_left, float width, float height, float z, b2World * world){

    float box2d_xpos = x_top_left + width/2.0f;
    float box2d_ypos = y_top_left - height/2.0f;

    b2BodyDef bodydef;

    bodydef.position.Set(0,0);
    bodydef.type = b2_staticBody;
    body=world->CreateBody(&bodydef);

    body->SetUserData((RenderableObject *)this);

    b2PolygonShape shape;
    shape.SetAsBox(width/2,height/2);


    //LEVEL LOAD
    mapFilePath = getAppConfigFilePath() + "/CapitanAfrica.map";
    mapFileOpenErrno = level.loadLevelFromFile(mapFilePath);
    if(mapFileOpenErrno)
    {
        mapFileOpenErrorString = strerror(mapFileOpenErrno);
    }

    LS_init(&lineStripRenderer, level.ground_verticles.data(), level.ground_verticles.size());


    b2Vec2 * vs;
    vs = (b2Vec2 *)malloc(level.ground_verticles.size()*sizeof(b2Vec2));
    for(unsigned int i =0; i < level.ground_verticles.size(); i++)
    {
        glm::vec3 punkt = level.ground_verticles.at(i);
        vs[i].Set(punkt.x, punkt.y);
    }

    b2ChainShape chain;
    chain.CreateChain(vs, level.ground_verticles.size());
    free(vs);

    b2FixtureDef fixturedef;
    fixturedef.shape=&chain;
    fixturedef.density=1.0;
    body->CreateFixture(&fixturedef);


    GLfloat x_bottom_right = x_top_left + width;
    GLfloat y_bottom_right = y_top_left - height;

}

GroundChain::~GroundChain(){
    LS_delete(&lineStripRenderer);
}

void GroundChain::render(glm::mat4 projection, glm::mat4 view){
    //cout << "Ground::render()" << endl;
    lineStripRenderer.projection = projection;
    lineStripRenderer.view = view;
    lineStripRenderer.model = glm::mat4(1);

    LS_draw(&lineStripRenderer, 10.0f);
}

