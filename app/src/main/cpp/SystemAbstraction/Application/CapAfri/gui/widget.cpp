#include "widget.hpp"
#include <library_opengles_2/Resources/Resources.hpp>
#include <library_opengles_2/ShaderManager/shader_manager.hpp>
#include <library_opengles_2/Shader/ShadersSources/colour_shader_source.hpp>
#include <iostream>

using namespace std;

Widget::Widget()
{ 
    mDimm = glm::vec2(246,133);
    shader = ShaderManager::getInstance()->getShaderFromSource("colour_shader_source.hpp", colour_vertex_shader_source, colour_fragment_shader_source);

    glm::vec3 verticles[4];

    verticles[0] = glm::vec3(-246/2, 133/2, 0);
    verticles[1] = glm::vec3(-246/2, -133/2, 0);
    verticles[2] = glm::vec3(246/2, -133/2, 0);
    verticles[3] = glm::vec3(246/2, 133/2, 0);

    DE_initRectangle_8(&background_rectangle, verticles);
    DE_setShader(&background_rectangle, shader);
}

Widget::~Widget()
{

}

void Widget::setModel(glm::mat4 model)
{
    mModel = model;
}

void Widget::setDimm(glm::vec2 dim)
{
    mDimm = dim;
    DE_setDimm(&background_rectangle, mDimm);
}

void Widget::setMatrices(glm::vec4 * viewport, glm::mat4 * projection, glm::mat4 * view)
{
    mViewport = viewport;
    mProjection = projection;
    mView = view;
}

void Widget::setEventListener(WidgetEventListener * listener)
{
    mListener = listener;
}

void Widget::Render()
{
    background_rectangle.projection = *mProjection;
    background_rectangle.view = *mView;
    background_rectangle.model = mModel;

    DE_drawRectangle(&background_rectangle,GL_LINE_LOOP);
}

