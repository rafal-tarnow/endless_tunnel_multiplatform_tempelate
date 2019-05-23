#include "coin_circle_body.hpp"

#include <iostream>
#include <library_opengles_2/TextureManager/texture_manager.hpp>
#include <library_opengles_2/ShaderManager/shader_manager.hpp>
#include <library_opengles_2/Shader/ShadersSources/texture_shader_source.hpp>
#include "../../../system_log.hpp"

using namespace std;


//*********     CIRCLE_COIN_RENDERER ***************

uint32_t CircleCoinRender::instancesCount = 0;
GLuint CircleCoinRender::coinTextureId = 0;
DE_Rectangle CircleCoinRender::rectangle;

CircleCoinRender::CircleCoinRender(float x, float y, float z, float radius)
{
    mPos.x = x;
    mPos.y = y;
    mPos.z = z;
    m_radius = radius;

    instancesCount++;
    //LOGD("CircleCoinRender::instancesCount = %d\n", instancesCount);

    if(instancesCount == 1)
    {
        coinTextureId = TextureManager::getInstance()->getTextureId("textures/coin_2.png");
        shader = ShaderManager::getInstance()->getShaderFromSource("texture_shader_source.hpp", texture_vertex_shader_source, texture_fragment_shader_source);
        DE_initRectangle_7(&rectangle, &coinTextureId, radius*2.0f, radius*2, mPos.z);
        DE_setShader(&rectangle, shader);
    }
}
CircleCoinRender::~CircleCoinRender()
{
    instancesCount--;
    //LOGD("CircleCoinRender::instancesCount = %d\n", instancesCount);

    if(instancesCount == 0)
    {
        DE_deleteRectangle(&rectangle);
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
    return mPos;
}

void CircleCoinRender::render(glm::mat4 projection, glm::mat4 view)
{
    rectangle.projection = projection;
    rectangle.view = view;

    b2Vec2 b_pos;
    b_pos.x = mPos.x;
    b_pos.y = mPos.y;
    drawCircleSquare(mPos,m_radius,0);
}

//************* CIRCLE COIN ***********************



CircleCoin::CircleCoin(float32 x,float32 y, float z, float32 radius, b2World* world) : CircleCoinRender(x, y, z, radius){
    GameObject::setObjectType(OBJECT_COIN);

    //BODY
    b2BodyDef bodydef;
    bodydef.position.Set(x,y);
    bodydef.type=b2_dynamicBody;
    bodydef.gravityScale = 0.0f;
    mBody=world->CreateBody(&bodydef);
    mBody->SetUserData(static_cast<GameObject*>(this));

    //SHAPE
    b2CircleShape shape;
    shape.m_radius = radius; //radius
    shape.m_p.Set(0,0); //position, relative to body position

    //FIXTURE
    b2FixtureDef fixturedef;
    fixturedef.shape=&shape;
    fixturedef.density=1.0;
    mBody->CreateFixture(&fixturedef);
}

CircleCoin::~CircleCoin(){
//    LOGD("delete CircleCoin nr: %d\n", instancesCount);
    mBody->GetWorld()->DestroyBody(mBody);
}

void CircleCoin::render(glm::mat4 projection, glm::mat4 view){
    rectangle.projection = projection;
    rectangle.view = view;

    glDisable(GL_DEPTH_TEST);
    
    drawCircleSquare(mBody->GetPosition(),((b2CircleShape*)mBody->GetFixtureList()->GetShape())->m_radius,mBody->GetAngle());
    
    glEnable(GL_DEPTH_TEST);
}
