#pragma once

#include <GLES2/gl2.h>


#include <Box2D/Box2D.h>
#include <library_opengles_2/RectangleRenderer/Rectangle_Renderer.hpp>
#include "../renderable_object.hpp"

class CircleCoinRender : public RenderableObject{
public:
    CircleCoinRender(float x, float y, float z, float radius);
    ~CircleCoinRender();
    void render(glm::mat4 projection, glm::mat4 view);
    void drawCircleSquare(b2Vec2 position,float radius,float angle);
    void drawCircleSquare(glm::vec3 position,float radius,float angle);

    glm::vec3 & getPosition();
protected:
    static DE_Rectangle rectangle;
private:
    static uint32_t instancesCount;
    static GLuint coinTextureId;

    float m_radius;
    glm::vec3 pos;
};


class CircleCoin: public CircleCoinRender{
public:
    CircleCoin(float32 x,float32 y, float z, float32 radius, b2World* world);
    ~CircleCoin();
    void render(glm::mat4 projection, glm::mat4 view);
private:
    b2Body* body = nullptr;
};
