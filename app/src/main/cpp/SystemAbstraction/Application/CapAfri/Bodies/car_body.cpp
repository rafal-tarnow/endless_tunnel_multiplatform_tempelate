#include "car_body.hpp"
#include <iostream>
#include <library_opengles_2/TextureManager/texture_manager.hpp>
#include <SOIL.h>
#include "../data/kolo.png.hpp"
#include "../data/car.png.hpp"

using namespace std;

void Car::drawCarWhell(b2Vec2 position,float radius,float angle)
{
    carWhellRectangle.model = glm::translate(glm::mat4(1), glm::vec3(position.x, position.y, 0.0));
    carWhellRectangle.model = glm::rotate(carWhellRectangle.model,angle,glm::vec3(0.0f,0.0f,1.0f));
    DE_drawRectangle(&carWhellRectangle);
}

void Car::drawCarBodyRectangle(b2Vec2* points,b2Vec2 position,float angle)
{
    carBodyRectangle.model = glm::translate(glm::mat4(1), glm::vec3(position.x, position.y, 0.0));
    carBodyRectangle.model = glm::rotate(carBodyRectangle.model,angle,glm::vec3(0.0f,0.0f,1.0f));
    DE_drawRectangle(&carBodyRectangle);
}

Car::Car(float32 x, float32 y, float z,  b2World * world)
{
    z_layer = z;
    //car body
    b2BodyDef car_bodydef;

    car_bodydef.position.Set(x,y);
    car_bodydef.type=b2_dynamicBody;

    carBody_body = world->CreateBody(&car_bodydef);
    carBody_body->SetUserData((RenderableObject *)this);

    b2PolygonShape carBodyShape;
    carBodyShape.SetAsBox(car_body_width_const/2.0f,car_body_height_const/2.0);

    b2FixtureDef fixturedef;
    fixturedef.shape = &carBodyShape;
    fixturedef.density = 0.3;
    carBody_body->CreateFixture(&fixturedef);

    //front whell body

    b2BodyDef whell_bodydef;
    whell_bodydef.position.Set(x+1,y-1);
    whell_bodydef.type = b2_dynamicBody;

    frontWhell_body = world->CreateBody(&whell_bodydef);

    whell_bodydef.position.Set(x-1,y-1);
    rearWhell_body = world->CreateBody(&whell_bodydef);

    b2CircleShape whellShape;
    whellShape.m_radius = whell_radius_const;
    whellShape.m_p.Set(0,0);

    b2FixtureDef whellFixture;
    whellFixture.shape = &whellShape;
    whellFixture.density = 1.0;
    whellFixture.friction = 10.0;

    frontWhell_body->CreateFixture(&whellFixture);
    rearWhell_body->CreateFixture(&whellFixture);

    b2Vec2 axis(0.0f, 1.0f);

    //przedni dzojt
    b2WheelJointDef whellJointDef;
    whellJointDef.Initialize(carBody_body, frontWhell_body,frontWhell_body->GetPosition(), axis);

    whellJointDef.collideConnected = false;
    whellJointDef.enableMotor = true;
    whellJointDef.motorSpeed = 0.0;
    whellJointDef.dampingRatio = 0.7f;
    whellJointDef.frequencyHz = 1.0f;
    whellJointDef.maxMotorTorque = 100.0f;

    frontWhellJoint = (b2WheelJoint*)world->CreateJoint( &whellJointDef );

    //tylny dzojt
    whellJointDef.Initialize(carBody_body,rearWhell_body,rearWhell_body->GetPosition(),axis);

    rearWhellJoint = (b2WheelJoint*)world->CreateJoint( &whellJointDef );

    //     //b2WheelJoint dzojt;



    bodyTextureId = SOIL_load_OGL_texture_from_memory(car_png, size_of_car_png, 4,0,SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y);
    DE_initRectangle(&carBodyRectangle,&bodyTextureId,car_body_width_const, car_body_height_const, z_layer);

    koloTextureId = SOIL_load_OGL_texture_from_memory(kolo_png, size_of_kolo_png, 4,0,SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y);
    DE_initRectangle(&carWhellRectangle, &koloTextureId, whell_radius_const*2.0f,whell_radius_const*2.0f, z_layer);

}

Car::~Car()
{
    DE_deleteRectangle(&carWhellRectangle);
    DE_deleteRectangle(&carBodyRectangle);

    glDeleteTextures(1, &koloTextureId);
    glDeleteTextures(1, &bodyTextureId);
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

void Car::render(glm::mat4 projection, glm::mat4 view)
{
   // frontWhellJoint->SetMotorSpeed(required_car_speed);
   //          rearWhellJoint->SetMotorSpeed(required_car_speed);

    //cout << "Car::render()" << endl;
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

    carBodyRectangle.projection = projection;
    carBodyRectangle.view = view;

    carWhellRectangle.projection = projection;
    carWhellRectangle.view = view;


    b2Vec2 points[4];

    for(int i=0;i<4;i++){
        points[i]=((b2PolygonShape*)carBody_body->GetFixtureList()->GetShape())->m_vertices[i];
    }

    glDisable(GL_DEPTH_TEST);

    drawCarBodyRectangle(points,carBody_body->GetPosition(),carBody_body->GetAngle());
    drawCarWhell(frontWhell_body->GetPosition(),((b2CircleShape*)frontWhell_body->GetFixtureList()->GetShape())->m_radius,frontWhell_body->GetAngle());
    drawCarWhell(rearWhell_body->GetPosition(),((b2CircleShape*)rearWhell_body->GetFixtureList()->GetShape())->m_radius,rearWhell_body->GetAngle());

    glEnable(GL_DEPTH_TEST);
}
