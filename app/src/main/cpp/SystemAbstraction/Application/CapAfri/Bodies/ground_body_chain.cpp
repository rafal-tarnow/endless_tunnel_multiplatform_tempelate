#include "ground_body_chain.hpp"

#include <iostream>
#include <library_opengles_2/TextureManager/texture_manager.hpp>

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


    b2Vec2 vs[6];
    vs[0].Set(-300.0f, 0.0f);
    vs[1].Set(10.0f, 2.0f);
    vs[2].Set(20.0f, 10.0f);
    vs[3].Set(45.f, 13.0f);
    vs[4].Set(58.0f, 0.0f);
    vs[5].Set(100.0f, 0.0f);

    b2ChainShape chain;
    chain.CreateChain(vs, 6);

    b2FixtureDef fixturedef;
    fixturedef.shape=&chain;
    fixturedef.density=1.0;
    body->CreateFixture(&fixturedef);


    GLfloat x_bottom_right = x_top_left + width;
    GLfloat y_bottom_right = y_top_left - height;


    float data[6*3] = {-300.0f,0.0f, 0.0f,
    10.0f, 2.0f, 0.0f,
    20.0f, 10.0f, 0.0f,
    45.f, 13.0f, 0.0f,
    58.0f, 0.0f, 0.0f,
    100.0f, 0.0f, 0.0f};
    LS_initLineStrip(&lineStripRenderer, data,6*3);
}

GroundChain::~GroundChain(){
    LS_deleteLineStrip(&lineStripRenderer);
}

void GroundChain::render(glm::mat4 projection, glm::mat4 view){
    //cout << "Ground::render()" << endl;
    lineStripRenderer.projection = projection;
    lineStripRenderer.view = view;
    lineStripRenderer.model = glm::mat4(1);

    LS_drawLineStrip(&lineStripRenderer, 10.0f);
}

