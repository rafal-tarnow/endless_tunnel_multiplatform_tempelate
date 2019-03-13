#pragma once

#include <library_opengles_2/RectangleRenderer/Rectangle_Renderer.hpp>
#include <string>
#include <library_opengles_2/TextRenderer/TextRenderer_v2.hpp>
#include <library_opengles_2/Debug/Debug.hpp>
#include <set>

using namespace std;

class Widget;

class WidgetEventListener{
public:
    virtual void Widget_onClicked(Widget * widget) = 0;
};

class Widget{
public:
    Widget();
    virtual ~Widget();

    void setModel(glm::mat4 model);
    void setVerticles(vector<glm::vec3> & verticles);
    void setDimm(glm::vec2 dimm);
    void setDrawBoundingBox(bool drawBoundingBox);

    void setNormalBackgroundTexture(GLuint textureId);
    void setPressedBackgroundTexture(GLuint textureId);

    void setEventListener(WidgetEventListener * listener);
    void addChild(Widget * );

    virtual void CustromDraw(){}

    void Render();
    void setMatrices(glm::vec4 &viewport, glm::mat4 &P, glm::mat4 &V);

    bool onPointerDown(float x_ndc, float y_ndc);
    void onPointerUp();

protected:
    glm::vec2 mDimm;
    bool isTouched = false;
    GLuint normalTexture = 0;
    GLuint touchedTexture = 0;
    set<Widget*> childs;

    glm::vec4 mViewport;
    glm::mat4 mProjection;
    glm::mat4 mView;
    glm::mat4 mLocalModel = glm::mat4(1);
    glm::mat4 mGlobalModel = glm::mat4(1);
    DE_Rectangle background_rectangle;

   void Draw(glm::mat4 &M);

private:
    bool mDbgDrawBoundingBox = false;
    bool mDbgDrawTouch = false;
    Shader_m * shader_dbg = nullptr;
    Shader_m * shader_texture = nullptr;
    WidgetEventListener * mListener = nullptr;

    DBG_COUNT("Widget");
};
