#pragma once

#include <system_opengl_include.hpp>

#include <Box2D/Box2D.h>
#include <library_opengles_2/RectangleRenderer/Rectangle_Renderer.hpp>
#include <library_opengles_2/Debug/Debug.hpp>
#include "../renderable_object.hpp"

class CarRenderer: public RenderableGameObject{
public:
    CarRenderer(glm::vec3 position);
    ~CarRenderer();

    void setPosition(glm::vec3 position);
    glm::vec3 getPosition();

    void render(glm::mat4 projection, glm::mat4 view);

protected:
    //BODY
   glm::vec3 mBodyPos = glm::vec3(0,0,0);
   float mBodyAngle = 0.0f;
   const float car_body_height_const = 1.0f;
   const float car_body_width_const = 3.0f;
   //FRONT WHELL
   glm::vec3 mFrontWhellPos = glm::vec3(0,0,0);
   float mFrontWhellRadius = 0.4f;
   float mFrontWhellAngle;
   //REAR WHELL
   glm::vec3 mRearWhellPos = glm::vec3(0,0,0);
   float mRearWhellRadius = 0.4f;
   float mRearWhellAngle;


private:
    void drawCarWhell(glm::vec3 position,float radius,float angle);
    void drawCarBodyRectangle(glm::vec3 position,float angle);



    DE_Rectangle carWhellRectangle;
    DE_Rectangle carBodyRectangle;
    Shader_m * shader;
    DBG_COUNT("CarRenderer");
};


class Car: public CarRenderer{
public:
    Car(glm::vec3 position, b2World * world, float dampingRatio = 0.7f, float frequencyHz = 1.0f, float maxMotorTorque = 100.0f, float friction = 10.0f);
    ~Car();
    float getSpeed();
    void setSpeed(float speed);
    void getPosition(float *x, float *y);
    b2Vec2 getPosition();
    float getXPosition();
    float getYPosition();

    void render(glm::mat4 projection, glm::mat4 view);
private:
    b2Body * carBody_body = nullptr;
    b2Body * frontWhell_body = nullptr;
    b2Body * rearWhell_body = nullptr;

    b2WheelJoint* frontWhellJoint = nullptr;
    b2WheelJoint* rearWhellJoint = nullptr;

    float current_car_speed = 0.0f;
    float required_car_speed = 0.0f;
    DBG_COUNT("Car");
};

