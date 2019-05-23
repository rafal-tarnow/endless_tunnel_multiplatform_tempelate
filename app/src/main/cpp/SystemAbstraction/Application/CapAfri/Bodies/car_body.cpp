#include "car_body.hpp"
#include <iostream>
#include <library_opengles_2/TextureManager/texture_manager.hpp>
#include <library_opengles_2/ShaderManager/shader_manager.hpp>
#include <library_opengles_2/Shader/ShadersSources/texture_shader_source.hpp>

using namespace std;

CarRenderer::CarRenderer(glm::vec3 position)
{
    GameObject::setObjectType(OBJECT_CAR);

    mBodyPos = position;
    mFrontWhellPos = glm::vec3(1,-1,0);
    mRearWhellPos = glm::vec3(-1,-1,0);

    shader = ShaderManager::getInstance()->getShaderFromSource("texture_shader_source.hpp", texture_vertex_shader_source, texture_fragment_shader_source);

    DE_initRectangle_5(&carBodyRectangle,"textures/car.png",car_body_width_const, car_body_height_const, mBodyPos.z);
    DE_setShader(&carBodyRectangle, shader);
    DE_initRectangle_5(&carWhellRectangle, "textures/kolo.png", mFrontWhellRadius*2.0f,mFrontWhellRadius*2.0f, mBodyPos.z);
    DE_setShader(&carWhellRectangle, shader);
}

CarRenderer::~CarRenderer()
{
    DE_deleteRectangle(&carWhellRectangle);
    DE_deleteRectangle(&carBodyRectangle);
}

void CarRenderer::drawCarWhell(glm::vec3 position,float radius,float angle)
{
    carWhellRectangle.model = glm::translate(glm::mat4(1), position);
    carWhellRectangle.model = glm::rotate(carWhellRectangle.model,angle,glm::vec3(0.0f,0.0f,1.0f));
    DE_drawRectangle(&carWhellRectangle);
}

void CarRenderer::drawCarBodyRectangle(glm::vec3 position,float angle)
{
    carBodyRectangle.model = glm::translate(glm::mat4(1), position);
    carBodyRectangle.model = glm::rotate(carBodyRectangle.model,angle,glm::vec3(0.0f,0.0f,1.0f));
    DE_drawRectangle(&carBodyRectangle);
}

void CarRenderer::setPosition(glm::vec3 position)
{
    mBodyPos = position;
    mFrontWhellPos = position  + glm::vec3(1,-1,0);
    mRearWhellPos = position + glm::vec3(-1,-1,0);
}

glm::vec3 CarRenderer::getPosition()
{
    return mBodyPos;
}

void CarRenderer::render(glm::mat4 projection, glm::mat4 view)
{

    glDisable(GL_DEPTH_TEST);

    carBodyRectangle.projection = projection;
    carBodyRectangle.view = view;

    carWhellRectangle.projection = projection;
    carWhellRectangle.view = view;

    drawCarBodyRectangle(mBodyPos, mBodyAngle);
    drawCarWhell(mFrontWhellPos,mFrontWhellRadius,mFrontWhellAngle);
    drawCarWhell(mRearWhellPos,mRearWhellRadius,mRearWhellAngle);

    glEnable(GL_DEPTH_TEST);
}


//********************* CAR **********************************


Car::Car(glm::vec3 position, b2World * world, float dampingRatio, float frequencyHz, float maxMotorTorque, float friction)
    :CarRenderer(position)
{
    //car body
    b2BodyDef car_bodydef;

    car_bodydef.position.Set(position.x,position.y);
    car_bodydef.type=b2_dynamicBody;

    carBody_body = world->CreateBody(&car_bodydef);
    carBody_body->SetUserData(static_cast<GameObject*>(this));

    b2PolygonShape carBodyShape;
    carBodyShape.SetAsBox(car_body_width_const/2.0f,car_body_height_const/2.0);

    b2FixtureDef fixturedef;
    fixturedef.shape = &carBodyShape;
    fixturedef.density = 0.3;
    carBody_body->CreateFixture(&fixturedef);

    //front whell body

    b2BodyDef whell_bodydef;
    whell_bodydef.position.Set(position.x+1,position.y-1);
    whell_bodydef.type = b2_dynamicBody;

    frontWhell_body = world->CreateBody(&whell_bodydef);
    frontWhell_body->SetUserData(static_cast<GameObject*>(this));

    whell_bodydef.position.Set(position.x-1,position.y-1);
    rearWhell_body = world->CreateBody(&whell_bodydef);
    rearWhell_body->SetUserData(static_cast<GameObject*>(this));

    b2CircleShape whellShape;
    whellShape.m_radius = mFrontWhellRadius;
    whellShape.m_p.Set(0,0);

    b2FixtureDef whellFixture;
    whellFixture.shape = &whellShape;
    whellFixture.density = 1.0;
    whellFixture.friction = friction;

    frontWhell_body->CreateFixture(&whellFixture);
    rearWhell_body->CreateFixture(&whellFixture);

    b2Vec2 axis(0.0f, 1.0f);

    //przedni dzojt
    b2WheelJointDef whellJointDef;
    whellJointDef.Initialize(carBody_body, frontWhell_body,frontWhell_body->GetPosition(), axis);

    whellJointDef.collideConnected = false;
    whellJointDef.enableMotor = true;
    whellJointDef.motorSpeed = 0.0;
    whellJointDef.dampingRatio = dampingRatio;
    whellJointDef.frequencyHz = frequencyHz;
    whellJointDef.maxMotorTorque = maxMotorTorque;

    frontWhellJoint = (b2WheelJoint*)world->CreateJoint(&whellJointDef );

    //tylny dzojt
    whellJointDef.Initialize(carBody_body,rearWhell_body,rearWhell_body->GetPosition(),axis);
    rearWhellJoint = (b2WheelJoint*)world->CreateJoint(&whellJointDef );

}

Car::~Car()
{
    carBody_body->GetWorld()->DestroyBody(carBody_body);
    frontWhell_body->GetWorld()->DestroyBody(frontWhell_body);
    rearWhell_body->GetWorld()->DestroyBody(rearWhell_body);
}
void Car::render(glm::mat4 projection, glm::mat4 view)
{
    if(required_car_speed == 0.0f)
    {
        if(current_car_speed > 0.0f)
        {
            current_car_speed = std::max(current_car_speed - 0.05f, required_car_speed);
            frontWhellJoint->SetMotorSpeed(current_car_speed);
            rearWhellJoint->SetMotorSpeed(current_car_speed);
        }else{
            current_car_speed = std::min(current_car_speed + 0.05f, required_car_speed);
            frontWhellJoint->SetMotorSpeed(current_car_speed);
            rearWhellJoint->SetMotorSpeed(current_car_speed);
        }

    }else if(required_car_speed < 0.0f)
    {
        current_car_speed = std::max(current_car_speed - 0.55f, required_car_speed);
        frontWhellJoint->SetMotorSpeed(current_car_speed);
        rearWhellJoint->SetMotorSpeed(current_car_speed);
    }else if(required_car_speed > 0.0f)
    {
        current_car_speed = std::min(current_car_speed + 0.55f, required_car_speed);
        frontWhellJoint->SetMotorSpeed(current_car_speed);
        rearWhellJoint->SetMotorSpeed(current_car_speed);
    }


    //UPDATE CAR BODY
    b2Vec2 carBodyPos = carBody_body->GetPosition();
    mBodyPos.x = carBodyPos.x;
    mBodyPos.y = carBodyPos.y;
    mBodyAngle = carBody_body->GetAngle();

    //FRONT WHELL POS
    b2Vec2 frontWhellPos = frontWhell_body->GetPosition();
    mFrontWhellPos.x = frontWhellPos.x;
    mFrontWhellPos.y = frontWhellPos.y;
    mFrontWhellAngle = frontWhell_body->GetAngle();

    //REAR WHELL POS
    b2Vec2 rearWhellPos = rearWhell_body->GetPosition();
    mRearWhellPos.x = rearWhellPos.x;
    mRearWhellPos.y = rearWhellPos.y;
    mRearWhellAngle = rearWhell_body->GetAngle();

    CarRenderer::render(projection, view);
}

float Car::getSpeed()
{
    return frontWhellJoint->GetMotorSpeed();
}

void Car::setSpeed(float speed)
{
    required_car_speed = speed;
}

void Car::getPosition(float *x, float *y)
{
    b2Vec2 pos = carBody_body->GetPosition();
    *x = pos.x;
    *y = pos.y;
}

b2Vec2 Car::getPosition()
{
    return carBody_body->GetPosition();
}

float Car::getXPosition(){
    return carBody_body->GetPosition().x;
}

float Car::getYPosition()
{
    return carBody_body->GetPosition().y;
}
