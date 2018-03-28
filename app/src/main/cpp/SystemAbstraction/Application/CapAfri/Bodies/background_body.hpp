#pragma once

#include <GLES2/gl2.h>

#include <Box2D/Box2D.h>
#include <library_opengles_2/RectangleRenderer/Rectangle_Renderer.hpp>
#include "../renderable_object.hpp"

class BackGround: public GameObject{
public:
    BackGround(float x_top_left, float y_top_left, float width, float height, b2World *  world);
    ~BackGround();

    void drawSquare(b2Vec2* points,b2Vec2 center,float angle,   GLuint texture);
    void render(glm::mat4 projection, glm::mat4 view);
private:
    DE_Rectangle rectangle;

};
