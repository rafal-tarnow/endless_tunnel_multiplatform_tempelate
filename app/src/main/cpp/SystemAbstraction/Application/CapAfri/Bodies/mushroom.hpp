#pragma once

#include <system_opengl_include.hpp>

#include <Box2D/Box2D.h>
#include <library_opengles_2/RectangleRenderer/Rectangle_Renderer.hpp>
#include "../renderable_object.hpp"

class MushroomRenderer : public RenderableGameObject{
public:
    MushroomRenderer(glm::vec3 position);
    ~MushroomRenderer();
    glm::vec3 & getPosition();
    void render(glm::mat4 projection, glm::mat4 view);

protected:
    static DE_Rectangle rectangle;
    static uint32_t instanceCount;
    glm::vec3 mPos;
    glm::vec2 mDimm;

private:
    static GLuint mushroomTextureId;
};


class Mushroom: public MushroomRenderer{
public:
    Mushroom(glm::vec3 position, b2World* world);
    ~Mushroom();
    void render(glm::mat4 projection, glm::mat4 view);
private:
    b2Body * mBody = nullptr;
};
