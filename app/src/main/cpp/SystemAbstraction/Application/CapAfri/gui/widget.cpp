#include "widget.hpp"
#include <iostream>
#include <library_opengles_2/Resources/Resources.hpp>
#include <library_opengles_2/Shader/ShadersSources/colour_shader_source.hpp>
#include <library_opengles_2/Shader/ShadersSources/texture_shader_source.hpp>
#include <library_opengles_2/ShaderManager/shader_manager.hpp>
#include <library_opengles_2/TextureManager/texture_manager.hpp>

using namespace std;

Widget::Widget()
{
    shader = ShaderManager::getInstance()->getShaderFromSource("colour_shader_source.hpp", colour_vertex_shader_source, colour_fragment_shader_source);

    vector<glm::vec3> verticles_vector;
    verticles_vector.push_back(glm::vec3(-246 / 2, 133 / 2, 0));
    verticles_vector.push_back(glm::vec3(-246 / 2, -133 / 2, 0));
    verticles_vector.push_back(glm::vec3(246 / 2, -133 / 2, 0));
    verticles_vector.push_back(glm::vec3(246 / 2, 133 / 2, 0));

    DE_initRectangle_8(&background_rectangle, verticles_vector);
    DE_setShader(&background_rectangle, shader);
}

Widget::~Widget()
{
    DE_deleteRectangle(&background_rectangle);
}

void Widget::setModel(glm::mat4 model)
{
    mLocalModel = model;
}

// void Widget::setVerticles(vector<glm::vec3> & verticles)
//{
//    DE_setVerticles(&background_rectangle, verticles);
//}

void Widget::setDimm(glm::vec2 dimm)
{
    mDimm = dimm;
    DE_setDimm(&background_rectangle, dimm);
}

void Widget::setEventListener(WidgetEventListener *listener)
{
    mListener = listener;
}

void Widget::addChild(Widget *child)
{
    childs.insert(child);
}

void Widget::Render(glm::vec4 &viewport, glm::mat4 &P, glm::mat4 &V, glm::mat4 &M)
{
    mViewport = viewport;
    mProjection = P;
    mView = V;
    mGlobalModel = M * mLocalModel;

    background_rectangle.projection = P;
    background_rectangle.view = V;
    background_rectangle.model = mGlobalModel;

    if (isTouched)
    {
        DE_setColour(&background_rectangle, glm::vec4(1, 0, 0, 1));
        DE_drawRectangle(&background_rectangle, GL_TRIANGLE_FAN);
    }
    DE_setColour(&background_rectangle, glm::vec4(0, 1, 0, 1));
    DE_drawRectangle(&background_rectangle, GL_LINE_LOOP);

    for (auto it = childs.begin(); it != childs.end(); ++it)
    {
        (*it)->Render(viewport, P, V, mGlobalModel);
    }
}

bool Widget::onPointerDown(float x_ndc, float y_ndc)
{
    cout << "Button::onPointerDown x = " << x_ndc << " y = " << y_ndc << endl;

    bool isChildTouch = false;

    if (childs.size())
    {
        for (auto it = childs.begin(); it != childs.end(); ++it)
        {
            if ((*it)->onPointerDown(x_ndc, y_ndc))
                isChildTouch = true;
        }
    }

    if (isChildTouch == false)
    {
        glm::vec3 window_position(x_ndc, mViewport.w - y_ndc, 0.0f);
        glm::vec3 world_position = glm::unProject(window_position, mGlobalModel * mView, mProjection, mViewport);

        cout << "world_position.x = " << world_position.x << " world_position.y = " << world_position.y << endl;

        if ((world_position.x >= (-mDimm.x / 2.0f)) && (world_position.x <= (mDimm.x / 2.0f)) && (world_position.y >= (-mDimm.y / 2.0f)) && (world_position.y <= (mDimm.y / 2.0f)))
        {
            isTouched = true;
            return true;
        }
        return false;
    }
    else
    {
        return isChildTouch;
    }
}

void Widget::onPointerUp()
{
    if (childs.size())
    {
        for (auto it = childs.begin(); it != childs.end(); ++it)
        {
            (*it)->onPointerUp();
        }
    }

    if (isTouched == true)
    {
        isTouched = false;
    }
}
