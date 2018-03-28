#pragma once

#include <GLES2/gl2.h>

#include <Box2D/Box2D.h>
#include <library_opengles_2/RectangleRenderer/Rectangle_Renderer.hpp>
#include "../renderable_object.hpp"

class Car: public GameObject{
public:
    Car(float32 x, float32 y, float z, b2World * world);
    ~Car();


    float getSpeed();
    void setSpeed(float speed);

    void getPosition(float *x, float *y);
    b2Vec2 getPosition();
    float getXPosition();
    float getYPosition();

    void render(glm::mat4 projection, glm::mat4 view);
private:
    void drawCarWhell(b2Vec2 position,float radius,float angle);
    void drawCarBodyRectangle(b2Vec2* points,b2Vec2 center,float angle);
    const float whell_radius_const = 0.4f;
    const float car_body_height_const = 1.0f;
    const float car_body_width_const = 3.0f;
    b2Body * carBody_body = nullptr;
    b2Body * frontWhell_body = nullptr;
    b2Body * rearWhell_body = nullptr;

    b2WheelJoint* frontWhellJoint = nullptr;
    b2WheelJoint* rearWhellJoint = nullptr;

    DE_Rectangle carWhellRectangle;
    DE_Rectangle carBodyRectangle;

    float current_car_speed = 0.0f;
    float required_car_speed = 0.0f;

    GLuint koloTextureId = 0;
    GLuint bodyTextureId = 0;

    float z_layer = 0.0f;
};
