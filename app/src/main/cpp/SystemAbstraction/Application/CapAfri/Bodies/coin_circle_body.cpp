#include "coin_circle_body.hpp"

#include <iostream>
#include <library_opengles_2/TextureManager/texture_manager.hpp>
#include "../data/coin_2.png.hpp"
#include <SOIL.h>
#include "../../../system_log.hpp"

using namespace std;


static DE_Rectangle rectangle;

bool CircleCoin::rectangleInited = false;

void CircleCoin::drawCircleSquare(b2Vec2 position,float radius,float angle)
{
    rectangle.model = glm::translate(glm::mat4(1.0f), glm::vec3(position.x, position.y, 0.0f));
    rectangle.model = glm::rotate(rectangle.model, angle, glm::vec3(0.0f, 0.0f, 1.0f));
    DE_drawRectangle(&rectangle);
}

CircleCoin::CircleCoin(float32 x,float32 y,float32 radius, b2World* world){
    b2BodyDef bodydef;
    bodydef.position.Set(x,y);
    bodydef.type=b2_dynamicBody;
    bodydef.gravityScale = 0.0f;
    body=world->CreateBody(&bodydef);
    body->SetUserData((RenderableObject *)this);
    b2CircleShape shape;
    shape.m_radius = radius; //radius
    shape.m_p.Set(0,0); //position, relative to body position

    b2FixtureDef fixturedef;
    fixturedef.shape=&shape;
    fixturedef.density=1.0;
    body->CreateFixture(&fixturedef);
    if(rectangleInited == false)
    {
        LOGD("1.9.4.1.9");
        coinTextureId = SOIL_load_OGL_texture_from_memory(coin_2_png, size_of_coin_2_png, 4,0,SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y);
        DE_initRectangle(&rectangle, &coinTextureId, radius*2.0f, radius*2, 0.0f);
        rectangleInited = true;
        LOGD("1.9.4.1.10");
    }
    LOGD("1.9.4.1.11");
}

CircleCoin::~CircleCoin(){
    if(rectangleInited == true)
    {
        DE_deleteRectangle(&rectangle);
        rectangleInited = false;
        glDeleteTextures(1, &coinTextureId);
    }
}

void CircleCoin::render(glm::mat4 projection, glm::mat4 view){
    rectangle.projection = projection;
    rectangle.view = view;

    drawCircleSquare(body->GetPosition(),((b2CircleShape*)body->GetFixtureList()->GetShape())->m_radius,body->GetAngle());
}
