#pragma once

#include <library_opengles_2/RectangleRenderer/Rectangle_Renderer.hpp>
#include <string>
#include <library_opengles_2/TextRenderer/TextRenderer_v2.hpp>
#include <library_opengles_2/Debug/Debug.hpp>

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
    void setPressed(bool pressed);
    void setMatrices(glm::vec4 * viewport, glm::mat4 * projection, glm::mat4 * view);

    void setEventListener(WidgetEventListener * listener);

    virtual void Render();

protected:
    vector<Widget*> childs;
    glm::vec4 *mViewport;
    glm::mat4 *mProjection;
    glm::mat4 *mView;

    glm::mat4 mModel;
    DE_Rectangle background_rectangle;

private:
    Shader_m * shader;
    WidgetEventListener * mListener = nullptr;

    DBG_COUNT("Widget");
};
