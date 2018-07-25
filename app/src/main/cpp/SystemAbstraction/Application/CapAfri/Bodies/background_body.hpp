#pragma once

#include <GLES2/gl2.h>

#include <Box2D/Box2D.h>
#include <library_opengles_2/RectangleRenderer/Rectangle_Renderer.hpp>
#include "../renderable_object.hpp"

class BackGround: public RenderableGameObject{
public:
    BackGround(float x_top_left, float y_top_left, float width, float height, uint32_t image_index);
    ~BackGround();
    void setModel(const glm::mat4 & model);
    void render(glm::mat4 projection, glm::mat4 view);
private:
    DE_Rectangle rectangle;
    glm::mat4 mModel = glm::mat4(1);

};
