#pragma once

#include <GLES2/gl2.h>


#include <Box2D/Box2D.h>
#include <library_opengles_2/RectangleRenderer/Rectangle_Renderer.hpp>
#include "../renderable_object.hpp"

class CircleCoin: public RenderableObject{
public:
    CircleCoin(float32 x,float32 y,float32 radius, b2World* world);
    ~CircleCoin();
    void render(glm::mat4 projection, glm::mat4 view);
private:
    void drawCircleSquare(b2Vec2 position,float radius,float angle);
    b2Body* body = nullptr;

    static bool rectangleInited;
    GLuint coinTextureId = 0;

};
