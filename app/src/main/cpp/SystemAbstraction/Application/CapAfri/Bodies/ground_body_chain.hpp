#pragma once

#include <GLES2/gl2.h>

#include <Box2D/Box2D.h>
#include <library_opengles_2/RectangleRenderer/Primitive_Renderer.hpp>
#include "../renderable_object.hpp"
#include "../level.hpp"
#include <library_opengles_2/RectangleRenderer/TriangleStrip_Renderer.hpp>
#include <library_opengles_2/Debug/Debug.hpp>

class GroundChain: public RenderableGameObject, public ObjectCounter{
public:
    GroundChain(Level & level, float x_top_left, float y_top_left, float width, float height, GLfloat z, b2World *  world);
    ~GroundChain();

    void render(glm::mat4 projection, glm::mat4 view);
private:
    b2Body * body = nullptr;
    Primitive lineStripRenderer;
    TS_TriangleStrip triangleStrip;

     GLuint coinTextureId;
};
