#pragma once

#include <library_opengles_2/RectangleRenderer/Rectangle_Renderer.hpp>
#include <string>

using namespace std;

class Button{
  public:
    Button();
    ~Button();

    void setPosition(glm::vec3 &position);
    void setMatrices(glm::vec4 *Viewport, glm::mat4 *Projection, glm::mat4 *View);

    void Render();

    void onPointerDown(float x_ndc, float y_ndc);
    void onPointerUp();

    void setNormalBackgroundTexture(GLuint textureId);
    void setPressedBackgroundTexture(GLuint textureId);

private:
    glm::vec4 *mViewport;
    glm::mat4 *mProjection;
    glm::mat4 *mView;
    glm::mat4 mModel;

    bool isTouched = false;

    glm::vec3 mPosition;
    glm::vec2 mDimm;

    DE_Rectangle rectangle;
    GLuint normalTexture;
    GLuint touchedTexture;
};
