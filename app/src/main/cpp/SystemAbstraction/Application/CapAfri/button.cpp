#include "button.hpp"
#include <library_opengles_2/Resources/Resources.hpp>

Button::Button()
{
    glm::vec3 position(0,0,0.0f);
    glm::vec2 dimm(0.2,0.2);

    string textureName("textures/shock_absorber.png");

    DE_initRectangle(&rectangle,textureName.c_str(),position,dimm);
}

Button::~Button()
{
    DE_deleteRectangle(&rectangle);
}

void Button::Render(glm::mat4 P, glm::mat4 V, glm::mat4 M)
{
    rectangle.projection = P;
    rectangle.view = V;
    rectangle.model = M;
    DE_drawRectangle(&rectangle);
}
