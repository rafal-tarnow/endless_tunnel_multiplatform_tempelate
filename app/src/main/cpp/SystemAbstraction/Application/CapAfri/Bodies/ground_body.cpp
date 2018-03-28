#include "ground_body.hpp"

#include <iostream>
#include <library_opengles_2/TextureManager/texture_manager.hpp>
#include <SOIL.h>
#include "../data/ground.png.hpp"

using namespace std;


Ground::Ground(float x_top_left, float y_top_left, float width, float height, float z, b2World * world){

    float box2d_xpos = x_top_left + width/2.0f;
    float box2d_ypos = y_top_left - height/2.0f;

    b2BodyDef bodydef;

    bodydef.position.Set(box2d_xpos,box2d_ypos);
    body=world->CreateBody(&bodydef);

    body->SetUserData((GameObject *)this);

    b2PolygonShape shape;
    shape.SetAsBox(width/2,height/2);

    b2FixtureDef fixturedef;
    fixturedef.shape=&shape;
    fixturedef.density=1.0;
    body->CreateFixture(&fixturedef);


    GLfloat x_bottom_right = x_top_left + width;
    GLfloat y_bottom_right = y_top_left - height;


    groundTextureId = SOIL_load_OGL_texture_from_memory(ground_png, size_of_ground_png, 4,0,SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y);

    DE_initRectangle_1(&rectangle, groundTextureId, x_top_left, y_top_left, x_bottom_right, y_bottom_right, 0.0f);
}

Ground::~Ground(){
    DE_deleteRectangle(&rectangle);
    glDeleteTextures(1,&groundTextureId);
}

void Ground::render(glm::mat4 projection, glm::mat4 view){
    //cout << "Ground::render()" << endl;
    rectangle.projection = projection;
    rectangle.view = view;
    rectangle.model = glm::mat4(1);

    DE_drawRectangle(&rectangle);
}

