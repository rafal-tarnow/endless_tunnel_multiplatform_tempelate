#pragma once

#include <GLES2/gl2.h>


#include <Box2D/Box2D.h>
#include <library_opengles_2/RectangleRenderer/Rectangle_Renderer.hpp>
#include "../renderable_object.hpp"
#include <library_opengles_2/Debug/Debug.hpp>

class MetaRenderer : public RenderableGameObject{
public:
    MetaRenderer(glm::vec3, float radius);
    ~MetaRenderer();

    void setPosAndDimm(glm::vec3, float radius);

    void render(glm::mat4 projection, glm::mat4 view);
    void drawCircleSquare(b2Vec2 position,float radius,float angle);
    void drawCircleSquare(glm::vec3 position,float radius,float angle);

    glm::vec3 & getPosition();
protected:
    static DE_Rectangle rectangle;
    static uint32_t instancesCount;
    glm::vec3 pos;
private:
    Shader_m * shader;
    static GLuint coinTextureId;
    float m_radius;
    DBG_COUNT("MetaRenderer");
};


class Meta: public MetaRenderer{
public:
    Meta(glm::vec3 position, float32 radius, b2World* world);
    ~Meta();
    void render(glm::mat4 projection, glm::mat4 view);
private:
    b2Body* body = nullptr;
    DBG_COUNT("Meta");
};
