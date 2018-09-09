#pragma once

#include <library_opengles_2/RectangleRenderer/Rectangle_Renderer.hpp>
#include <string>
#include <library_opengles_2/TextRenderer/TextRenderer_v2.hpp>

using namespace std;

class Button;

class ButtonEventListener{
  public:
    virtual void Button_onClicked(Button * button) = 0;
};

class Button{
  public:
    Button();
    virtual ~Button();

    void setText(string text);
    string getText();
    void setPosition(glm::vec3 &position);
    void setModel(glm::mat4 model);
    void setDimm(glm::vec2 dim);
    void setMatrices(glm::vec4 *Viewport, glm::mat4 *Projection, glm::mat4 *View);

    void setEventListener(ButtonEventListener * listener);

    virtual void Render();

    bool onPointerDown(float x_ndc, float y_ndc);
    void onPointerUp();

    void setNormalBackgroundTexture(GLuint textureId);
    void setPressedBackgroundTexture(GLuint textureId);

protected:
      bool isTouched = false;
      GLuint normalTexture;
      GLuint touchedTexture;
      glm::mat4 *mProjection;
      glm::mat4 *mView;
      glm::mat4 mModel;
      DE_Rectangle rectangle;



private:
    string mText;
    ButtonEventListener * mListener = nullptr;

    TextRenderer_v2 * textRenderer_v2 = nullptr;

    glm::vec4 *mViewport;
    glm::vec3 mPosition;
    glm::vec2 mDimm;


};
