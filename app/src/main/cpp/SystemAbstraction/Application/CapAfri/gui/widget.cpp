#include "widget.hpp"
#include <library_opengles_2/Resources/Resources.hpp>
#include <library_opengles_2/ShaderManager/shader_manager.hpp>
#include <library_opengles_2/Shader/ShadersSources/colour_shader_source.hpp>
#include <library_opengles_2/Shader/ShadersSources/texture_shader_source.hpp>
#include <library_opengles_2/TextureManager/texture_manager.hpp>
#include <iostream>

using namespace std;

Widget::Widget()
{ 
    shader = ShaderManager::getInstance()->getShaderFromSource("colour_shader_source.hpp", colour_vertex_shader_source, colour_fragment_shader_source);

    vector<glm::vec3> verticles_vector;
    verticles_vector.push_back(glm::vec3(-246/2, 133/2, 0));
    verticles_vector.push_back(glm::vec3(-246/2, -133/2, 0));
    verticles_vector.push_back(glm::vec3(246/2, -133/2, 0));
    verticles_vector.push_back(glm::vec3(246/2, 133/2, 0));

    DE_initRectangle_8(&background_rectangle, verticles_vector);
    DE_setShader(&background_rectangle, shader);
}

Widget::~Widget()
{
    DE_deleteRectangle(&background_rectangle);
}

void Widget::setModel(glm::mat4 model)
{
    mModel = model;
}

void Widget::setVerticles(vector<glm::vec3> & verticles)
{
    DE_setVerticles(&background_rectangle, verticles);
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

