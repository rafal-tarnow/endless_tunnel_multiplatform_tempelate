#include "button.hpp"


Button::Button()
{
    glm::vec3 position(0,0,0.0f);
    glm::vec2 dimm(1,1);
    string textureName("./GRAPHICS/shock_absorber.png");
    DE_initRectangle(&rectangle,textureName.c_str(),position,dimm);
    //DE_initRectangle(&rectangle,textureName.c_str(), 300, 400, 0);
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
