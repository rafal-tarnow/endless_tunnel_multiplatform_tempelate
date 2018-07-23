#include "mushroom.hpp"
#include <library_opengles_2/TextureManager/texture_manager.hpp>


uint32_t MushroomRenderer::instanceCount = 0;
GLuint MushroomRenderer::mushroomTextureId = 0;
DE_Rectangle MushroomRenderer::rectangle;

//*********     MUSHROOM_RENDERER ***************

MushroomRenderer::MushroomRenderer(glm::vec3 position)
{
    GameObject::setObjectType(OBJECT_MUSHROOM);

    mPos = position;
    mDimm = glm::vec2(1.0, 1.0);

    instanceCount++;

    if(instanceCount == 1)
    {
        mushroomTextureId = TextureManager::getInstance()->getTextureId("textures/Tango_Style_Mushroom_icon.svg.png");
        DE_initRectangle(&rectangle, mushroomTextureId, mDimm);
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

glm::vec3 & MushroomRenderer::getPosition()
{
    return mPos;
}

void MushroomRenderer::render(glm::mat4 projection, glm::mat4 view)
{
    rectangle.projection = projection;
    rectangle.view = view;
    rectangle.model = glm::translate(glm::mat4(1), mPos);

    DE_drawRectangle(&rectangle);
}


//************* MUSHROOM COIN ***********************

Mushroom::Mushroom(glm::vec3 position, b2World* world)
    : MushroomRenderer(position)
{

    //BODY
    b2BodyDef bodydef;
    bodydef.position.Set(position.x, position.y);
    bodydef.type = b2_staticBody;
    mBody = world->CreateBody(&bodydef);
    mBody->SetUserData(static_cast<GameObject*>(this));

    //SHPE
    b2PolygonShape shape;
    shape.SetAsBox(mDimm.x/2.0, mDimm.y/2.0, b2Vec2(0, 0), 0.0f);

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

