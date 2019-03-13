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
    // GEOMETRY
    vector<glm::vec3> verticles_vector;
    verticles_vector.push_back(glm::vec3(-246 / 2, 133 / 2, 0));
    verticles_vector.push_back(glm::vec3(-246 / 2, -133 / 2, 0));
    verticles_vector.push_back(glm::vec3(246 / 2, -133 / 2, 0));
    verticles_vector.push_back(glm::vec3(246 / 2, 133 / 2, 0));

    // DEBUG
    shader_dbg = ShaderManager::getInstance()->getShaderFromSource("colour_shader_source.hpp", colour_vertex_shader_source, colour_fragment_shader_source);




    // TEXTURE BACKGROUND
    shader_texture = ShaderManager::getInstance()->getShaderFromSource("texture_shader_source.hpp", texture_vertex_shader_source, texture_fragment_shader_source);

    DE_initRectangle_8(&background_rectangle, verticles_vector);
    //DE_setShader(&background_rectangle, shader_texture);
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

void Widget::setNormalBackgroundTexture(GLuint textureId)
{
    normalTexture = textureId;
    if (isTouched == false)
    {
        background_rectangle.texture_id = normalTexture;
    }
}

void Widget::setPressedBackgroundTexture(GLuint textureId)
{
    touchedTexture = textureId;
    if (isTouched == true)
    {
        background_rectangle.texture_id = touchedTexture;
    }
}

void Widget::setDrawBoundingBox(bool drawBoundingBox)
{
    mDbgDrawBoundingBox = drawBoundingBox;
}

void Widget::setEventListener(WidgetEventListener *listener)
{
    mListener = listener;
}

void Widget::addChild(Widget *child)
{
    childs.insert(child);
}

void Widget::setMatrices(glm::vec4 &viewport, glm::mat4 &P, glm::mat4 &V)
{
    mViewport = viewport;
    mProjection = P;
    mView = V;

    background_rectangle.projection = mProjection;
    background_rectangle.view = mView;
    background_rectangle.model = mGlobalModel;

    for (auto it = childs.begin(); it != childs.end(); ++it)
    {
        (*it)->setMatrices(mViewport, mProjection, mView);
    }
}

void Widget::Render()
{
    glm::mat4 M(1);
    this->Draw(M);
}

void Widget::Draw(glm::mat4 &M)
{
    mGlobalModel = M * mLocalModel;

    
    if(normalTexture && !isTouched)
    {
        shader_texture->use();
        shader_texture->setMat4(shader_texture->projectionLocation, mProjection);
        shader_texture->setMat4(shader_texture->viewLocation, mView);
        shader_texture->setMat4(shader_texture->modelLocation, mGlobalModel);
        DE_drawRectangle(&background_rectangle, GL_TRIANGLE_FAN, shader_texture);
    }
    else if(touchedTexture && isTouched)
    {
        shader_texture->use();
        shader_texture->setMat4(shader_texture->projectionLocation, mProjection);
        shader_texture->setMat4(shader_texture->viewLocation, mView);
        shader_texture->setMat4(shader_texture->modelLocation, mGlobalModel);
        DE_drawRectangle(&background_rectangle, GL_TRIANGLE_FAN, shader_texture);
    }
    
    CustromDraw();

    
    shader_dbg->use();
    shader_dbg->setMat4(shader_dbg->projectionLocation, mProjection);
    shader_dbg->setMat4(shader_dbg->viewLocation, mView);
    shader_dbg->setMat4(shader_dbg->modelLocation, mGlobalModel);

    if (isTouched && mDbgDrawTouch)
    {
        shader_dbg->setVec4(shader_dbg->colourLocation, glm::vec4(1, 0, 0, 1));
        DE_drawRectangle(&background_rectangle, GL_TRIANGLE_FAN,shader_dbg);
    }
    if (mDbgDrawBoundingBox)
    {
        shader_dbg->setVec4(shader_dbg->colourLocation, glm::vec4(0, 1, 0, 1));
        DE_drawRectangle(&background_rectangle, GL_LINE_LOOP,shader_dbg);
    }

    for (auto it = childs.begin(); it != childs.end(); ++it)
    {
        (*it)->Draw(mGlobalModel);
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
            background_rectangle.texture_id = touchedTexture;
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
        background_rectangle.texture_id = normalTexture;
    }
}
