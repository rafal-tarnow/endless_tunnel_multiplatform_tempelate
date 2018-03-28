#pragma once

#include <GLES2/gl2.h>

#include <Box2D/Box2D.h>
#include <library_opengles_2/RectangleRenderer/Rectangle_Renderer.hpp>
#include "../renderable_object.hpp"

class Ground: public GameObject{
public:
    Ground(float x_top_left, float y_top_left, float width, float height, GLfloat z, b2World *  world);
    ~Ground();

    void render(glm::mat4 projection, glm::mat4 view);
private:
    b2Body * body = nullptr;
    DE_Rectangle rectangle;
    GLuint groundTextureId = 0;

};
