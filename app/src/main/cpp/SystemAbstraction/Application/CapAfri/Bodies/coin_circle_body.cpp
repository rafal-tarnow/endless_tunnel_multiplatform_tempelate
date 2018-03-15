#include "coin_circle_body.hpp"

#include <iostream>
#include <library_opengles_2/TextureManager/texture_manager.hpp>
#include "../data/coin_2.png.hpp"
#include <SOIL.h>
#include "../../../system_log.hpp"

using namespace std;


//*********     CIRCLE_COIN_RENDERER ***************

uint32_t CircleCoinRender::instancesCount = 0;
GLuint CircleCoinRender::coinTextureId = 0;
DE_Rectangle CircleCoinRender::rectangle;

CircleCoinRender::CircleCoinRender(float x, float y, float radius)
{
    pos.x = x;
    pos.y = y;
    pos.z = 0.0f;
    m_radius = radius;

    instancesCount++;
    LOGD("CircleCoinRender::instancesCount = %d", instancesCount);

    if(instancesCount == 1)
    {
        coinTextureId = SOIL_load_OGL_texture_from_memory(coin_2_png, size_of_coin_2_png, 4,0,SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y);
        DE_initRectangle(&rectangle, &coinTextureId, radius*2.0f, radius*2, 0.0f);
    }
}
CircleCoinRender::~CircleCoinRender()
{
    instancesCount--;
    LOGD("CircleCoinRender::instancesCount = %d", instancesCount);

    if(instancesCount == 0)
    {
        DE_deleteRectangle(&rectangle);
        glDeleteTextures(1, &coinTextureId);
    }
}

void CircleCoinRender::drawCircleSquare(b2Vec2 position,float radius,float angle)
{
    rectangle.model = glm::translate(glm::mat4(1.0f), glm::vec3(position.x, position.y, 0.0f));
    rectangle.model = glm::rotate(rectangle.model, angle, glm::vec3(0.0f, 0.0f, 1.0f));
    DE_drawRectangle(&rectangle);
}

void CircleCoinRender::drawCircleSquare(glm::vec3 position,float radius,float angle)
{
    b2Vec2 b_pos;
    b_pos.x = position.x;
    b_pos.y = position.y;
    drawCircleSquare(b_pos, radius, angle);
}

glm::vec3 & CircleCoinRender::getPosition()
{
    return pos;
}

void CircleCoinRender::render(glm::mat4 projection, glm::mat4 view)
{
    rectangle.projection = projection;
    rectangle.view = view;

    b2Vec2 b_pos;
    b_pos.x = pos.x;
    b_pos.y = pos.y;
    drawCircleSquare(pos,m_radius,0);
}

//************* CIRCLE COIN ***********************



CircleCoin::CircleCoin(float32 x,float32 y,float32 radius, b2World* world) : CircleCoinRender(x,y, radius){
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

}

CircleCoin::~CircleCoin(){

}

void CircleCoin::render(glm::mat4 projection, glm::mat4 view){
    rectangle.projection = projection;
    rectangle.view = view;

    drawCircleSquare(body->GetPosition(),((b2CircleShape*)body->GetFixtureList()->GetShape())->m_radius,body->GetAngle());
}
