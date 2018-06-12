#include "mushroom.hpp"
#include <library_opengles_2/TextureManager/texture_manager.hpp>


uint32_t MushroomRenderer::instanceCount = 0;
GLuint MushroomRenderer::mushroomTextureId = 0;
DE_Rectangle MushroomRenderer::rectangle;

//*********     MUSHROOM_RENDERER ***************

MushroomRenderer::MushroomRenderer(glm::vec3 position, glm::vec2 dimm)
{
    mPos = position;
    mDimm = dimm;

    instanceCount++;

    if(instanceCount == 1)
    {
        mushroomTextureId = TextureManager::getTextureId("textures/Tango_Style_Mushroom_icon.svg.png");
        DE_initRectangle(&rectangle, mushroomTextureId, mPos, mDimm);
    }
}

MushroomRenderer::~MushroomRenderer()
{
    instanceCount--;

    if(instanceCount == 0)
    {
        DE_deleteRectangle(&rectangle);
    }
}

void MushroomRenderer::render(glm::mat4 projection, glm::mat4 view)
{
    rectangle.projection = projection;
    rectangle.view = view;
    rectangle.model = glm::translate(glm::mat4(1.0f),mPos);

    DE_drawRectangle(&rectangle);
}


//************* MUSHROOM COIN ***********************

Mushroom::Mushroom(glm::vec3 position, glm::vec2 dimm, b2World* world) : MushroomRenderer(position, dimm){
    GameObject::setObjectType(OBJECT_MUSHROOM);

    //BODY
    b2BodyDef bodydef;
    bodydef.position.Set(position.x, position.y);
    bodydef.type = b2_staticBody;
    mBody = world->CreateBody(&bodydef);
    mBody->SetUserData(static_cast<GameObject*>(this));

    //SHPE
    b2PolygonShape shape;
    shape.SetAsBox(dimm.x, dimm.y, b2Vec2(position.x, position.y), 0.0f);

    //FIXTURE
    b2FixtureDef fixturedef;
    fixturedef.shape=&shape;
    mBody->CreateFixture(&fixturedef);
}

Mushroom::~Mushroom()
{
    mBody->GetWorld()->DestroyBody(mBody);
}

void Mushroom::render(glm::mat4 projection, glm::mat4 view)
{
    b2Vec2 position = mBody->GetPosition();
    mPos.x = position.x;
    mPos.y = position.y;

    MushroomRenderer::render(projection, view);
}

