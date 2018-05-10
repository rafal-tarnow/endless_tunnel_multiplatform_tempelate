#pragma once

#include <library_opengles_2/RectangleRenderer/Rectangle_Renderer.hpp>
#include <string>

using namespace std;

class Button{
  public:
    Button();
    ~Button();
    void Render(glm::mat4 P, glm::mat4 V, glm::mat4 M);
private:
    DE_Rectangle rectangle;
};
