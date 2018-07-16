#include "meta_body.hpp"

#include <iostream>
#include <library_opengles_2/TextureManager/texture_manager.hpp>
#include "../../../system_log.hpp"

using namespace std;


//*********     CIRCLE_COIN_RENDERER ***************

uint32_t MetaRenderer::instancesCount = 0;
GLuint MetaRenderer::coinTextureId = 0;
DE_Rectangle MetaRenderer::rectangle;

MetaRenderer::MetaRenderer(float x, float y, float z, float radius)
{
    pos.x = x;
    pos.y = y;
    pos.z = z;
    m_radius = radius;

    instancesCount++;
    LOGD("CircleCoinRender::instancesCount = %d\n", instancesCount);

    if(instancesCount == 1)
    {
        coinTextureId = TextureManager::getTextureId("textures/meta.png");
        DE_initRectangle(&rectangle, &coinTextureId, radius*2.0f, radius*2, pos.z);
    }
}

void MetaRenderer::setPosAndDimm(float x, float y, float z, float radius)
{
    pos.x = x;
    pos.y = y;
    pos.z = z;
    m_radius = radius;
}

MetaRenderer::~MetaRenderer()
{
    instancesCount--;
    LOGD("CircleCoinRender::instancesCount = %d\n", instancesCount);

    if(instancesCount == 0)
    {
        DE_deleteRectangle(&rectangle);
    }
}

void MetaRenderer::drawCircleSquare(b2Vec2 position,float radius,float angle)
{
    rectangle.model = glm::translate(glm::mat4(1.0f), glm::vec3(position.x, position.y, 0.0f));
    rectangle.model = glm::rotate(rectangle.model, angle, glm::vec3(0.0f, 0.0f, 1.0f));
    DE_drawRectangle(&rectangle);
}

void MetaRenderer::drawCircleSquare(glm::vec3 position,float radius,float angle)
{
    b2Vec2 b_pos;
    b_pos.x = position.x;
    b_pos.y = position.y;
    drawCircleSquare(b_pos, radius, angle);
}

glm::vec3 & MetaRenderer::getPosition()
{
    return pos;
}

void MetaRenderer::render(glm::mat4 projection, glm::mat4 view)
{
    rectangle.projection = projection;
    rectangle.view = view;

    b2Vec2 b_pos;
    b_pos.x = pos.x;
    b_pos.y = pos.y;
    drawCircleSquare(pos,m_radius,0);
}

//************* CIRCLE COIN ***********************



Meta::Meta(float32 x,float32 y, float z, float32 radius, b2World* world) : MetaRenderer(x, y, z, radius){
    GameObject::setObjectType(OBJECT_COIN);

    b2BodyDef bodydef;
    bodydef.position.Set(x,y);
    bodydef.type=b2_dynamicBody;
    bodydef.gravityScale = 0.0f;
    body=world->CreateBody(&bodydef);
    body->SetUserData(static_cast<GameObject*>(this));
    b2CircleShape shape;
    shape.m_radius = radius; //radius
    shape.m_p.Set(0,0); //position, relative to body position

    b2FixtureDef fixturedef;
    fixturedef.shape=&shape;
    fixturedef.density=1.0;
    body->CreateFixture(&fixturedef);

}

Meta::~Meta(){
    LOGD("delete CircleCoin nr: %d\n", instancesCount);
    body->GetWorld()->DestroyBody(body);
}

void Meta::render(glm::mat4 projection, glm::mat4 view){
    rectangle.projection = projection;
    rectangle.view = view;

    glDisable(GL_DEPTH_TEST);
    
    drawCircleSquare(body->GetPosition(),((b2CircleShape*)body->GetFixtureList()->GetShape())->m_radius,body->GetAngle());
    
    glEnable(GL_DEPTH_TEST);
}
