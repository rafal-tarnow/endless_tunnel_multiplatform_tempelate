#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class RenderableObject{
public:
    virtual void render(glm::mat4 projection, glm::mat4 view) = 0;
};
