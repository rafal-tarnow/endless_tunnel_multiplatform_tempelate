#include "widget.hpp"
#include <library_opengles_2/Resources/Resources.hpp>
#include <library_opengles_2/ShaderManager/shader_manager.hpp>
#include <library_opengles_2/Shader/ShadersSources/texture_shader_source.hpp>
#include <iostream>

using namespace std;

Widget::Widget()
{ 
    mDimm = glm::vec2(246,133);

    shader = ShaderManager::getInstance()->getShaderFromSource("texture_shader_source.hpp", texture_vertex_shader_source, texture_fragment_shader_source);
    DE_initRectangle_3(&background_rectangle,normalTexture,mDimm);
    DE_setShader(&background_rectangle, shader);
    DE_initRectangle_3(&lock_rectangle,normalTexture,mDimm);
    DE_setShader(&lock_rectangle,shader);

}

Widget::~Widget()
{
    DE_deleteRectangle(&background_rectangle);
    DE_deleteRectangle(&lock_rectangle);

    if(textRenderer_v2)
        delete textRenderer_v2;
}

void Widget::setText(string text)
{
    mText = text;

    if(textRenderer_v2 == nullptr)
    {

        int height = 1080;
        int width = 1920;

        GLuint fontSize = GLuint(float(height)*0.076f);
        Resource font_design_graffiti_agentorange("fonts/design_graffiti_agentorange_www_myfontfree_com.ttf");
        //Resource font_design_graffiti_agentorange("fonts/arial.ttf");

        textRenderer_v2 = new TextRenderer_v2(width,height, glm::vec4(1,0,0,1));
        textRenderer_v2->LoadFromMemory("Design graffiti agentorange", font_design_graffiti_agentorange.getData(), font_design_graffiti_agentorange.getSize(), fontSize);
        textRenderer_v2->setColour(glm::vec4(0,0,0,1));
    }
}

string Widget::getText()
{
    return mText;
}

void Widget::setPosition(glm::vec3 &position)
{
    mPosition = position;
    mModel = glm::translate(glm::mat4(1),position);
}

void Widget::setModel(glm::mat4 model)
{
    mModel = model;
}

void Widget::setDimm(glm::vec2 dim)
{
    mDimm = dim;
    DE_setDimm(&background_rectangle, mDimm);
    DE_setDimm(&lock_rectangle, mDimm);
}

void Widget::setPressed(bool pressed)
{
    if(pressed == true)
    {
        isTouched = true;
        background_rectangle.texture_id = touchedTexture;
    }else
    {
        isTouched = false;
        background_rectangle.texture_id = normalTexture;
    }
}

void Widget::setLockable(bool lockable)
{
    isLockable = lockable;
}
void Widget::setLocked(bool lckd)
{
    locked = lckd;
}

void Widget::setMatrices(glm::vec4 *Viewport, glm::mat4 *Projection, glm::mat4 *View)
{
    mViewport = Viewport;
    mProjection = Projection;
    mView = View;
}

void Widget::setEventListener(WidgetEventListener * listener)
{
    mListener = listener;
}

void Widget::setNormalBackgroundTexture(GLuint textureId)
{
    normalTexture = textureId;
    if(isTouched == false)
    {
        background_rectangle.texture_id = normalTexture;
    }
}

void Widget::setPressedBackgroundTexture(GLuint textureId)
{
    touchedTexture = textureId;
    if(isTouched == true)
    {
        background_rectangle.texture_id = touchedTexture;
    }
}

void Widget::setLockTexture(GLuint textureId)
{
    lock_rectangle.texture_id = textureId;
}

void Widget::Render()
{
    background_rectangle.projection = *mProjection;
    background_rectangle.view = *mView;
    background_rectangle.model = mModel;

    DE_drawRectangle(&background_rectangle);



    if(mText != "")
    {
        glm::mat4 txt_model = glm::translate(mModel, glm::vec3(0,-20,0));


        textRenderer_v2->setCustomPV(*mProjection, *mView);
        textRenderer_v2->RenderText(1,mText, txt_model, TextRenderer_v2::TEXT_CENTER);
    }

    if((isLockable == true) && (locked == true))
    {
    lock_rectangle.projection = *mProjection;
    lock_rectangle.view = *mView;
    lock_rectangle.model = glm::scale(glm::translate(mModel,glm::vec3(50,-45,0)),glm::vec3(0.35, 0.35, 0.35));

    DE_drawRectangle(&lock_rectangle);
    }
}

bool Widget::onPointerDown(float x_ndc, float y_ndc)
{
    cout << "Button::onPointerDown x = " << x_ndc << " y = " <<  y_ndc << endl;

    glm::vec3 window_position(x_ndc, mViewport->w - y_ndc, 0.0f);
    glm::vec3 world_position = glm::unProject(window_position,mModel*(*mView), *mProjection, *mViewport);

    cout << "world_position.x = " << world_position.x << " world_position.y = " << world_position.y << endl;

    if((world_position.x >= (-mDimm.x/2.0f)) && (world_position.x <= (mDimm.x/2.0f)) && (world_position.y >= (-mDimm.y/2.0f)) && (world_position.y <= (mDimm.y/2.0f)))
    {
        isTouched = true;
        background_rectangle.texture_id = touchedTexture;
        return true;
    }
    return false;
}

void Widget::onPointerUp()
{
    if(isTouched == true)
    {
        isTouched = false;
        background_rectangle.texture_id = normalTexture;
        if(mListener)
        {
            mListener->Widget_onClicked(this);
        }
    }
}
