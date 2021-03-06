#include "button.hpp"
#include <iostream>
#include <library_opengles_2/Resources/Resources.hpp>
#include <library_opengles_2/Shader/ShadersSources/texture_shader_source.hpp>

using namespace std;

Button::Button()
{
    mDimm = glm::vec2(246, 133);

    DE_initRectangle_3(&background_rectangle, normalTexture, mDimm);
    DE_initRectangle_3(&lock_rectangle, normalTexture, mDimm);
    shader = ShaderManager::getInstance()->getShaderFromSource("texture_shader_source.hpp", texture_vertex_shader_source, texture_fragment_shader_source);
    DE_setShader(&background_rectangle, shader);
    DE_setShader(&lock_rectangle, shader);
}

Button::~Button()
{
    DE_deleteRectangle(&background_rectangle);
    DE_deleteRectangle(&lock_rectangle);

    if (textRenderer_v2)
        delete textRenderer_v2;
}

void Button::setText(string text)
{
    mText = text;

    if (textRenderer_v2 == nullptr)
    {

        int height = 1080;
        int width = 1920;

        GLuint fontSize = GLuint(float(height) * 0.076f);
        Resource font_design_graffiti_agentorange("fonts/design_graffiti_agentorange_www_myfontfree_com.ttf");
        // Resource font_design_graffiti_agentorange("fonts/arial.ttf");

        textRenderer_v2 = new TextRenderer_v2(width, height, glm::vec4(1, 0, 0, 1));
        textRenderer_v2->LoadFromMemory("design_graffiti_agentorange_www_myfontfree_com.ttf", font_design_graffiti_agentorange.getData(), font_design_graffiti_agentorange.getSize(), fontSize);
        textRenderer_v2->setColour(glm::vec4(0, 0, 0, 1));
    }
}

string Button::getText()
{
    return mText;
}

void Button::setPosition(glm::vec3 &position)
{
    mPosition = position;
    mModel = glm::translate(glm::mat4(1), position);
}

void Button::setModel(glm::mat4 model)
{
    mModel = model;
}

void Button::setDimm(glm::vec2 dim)
{
    mDimm = dim;
    DE_setDimm(&background_rectangle, mDimm);
    DE_setDimm(&lock_rectangle, mDimm);
}

void Button::setPressed(bool pressed)
{
    if (pressed == true)
    {
        isTouched = true;
        background_rectangle.texture_id = touchedTexture;
    }
    else
    {
        isTouched = false;
        background_rectangle.texture_id = normalTexture;
    }
}

void Button::setLockable(bool lockable)
{
    isLockable = lockable;
}
void Button::setLocked(bool lckd)
{
    locked = lckd;
}

bool Button::isLocked()
{
    return locked;
}

void Button::setMatrices(glm::vec4 *Viewport, glm::mat4 *Projection, glm::mat4 *View)
{
    mViewport = Viewport;
    mProjection = Projection;
    mView = View;
}

void Button::setEventListener(ButtonEventListener *listener)
{
    mListener = listener;
}

void Button::setNormalBackgroundTexture(GLuint textureId)
{
    normalTexture = textureId;
    if (isTouched == false)
    {
        background_rectangle.texture_id = normalTexture;
    }
}

void Button::setPressedBackgroundTexture(GLuint textureId)
{
    touchedTexture = textureId;
    if (isTouched == true)
    {
        background_rectangle.texture_id = touchedTexture;
    }
}

void Button::setLockTexture(GLuint textureId)
{
    lock_rectangle.texture_id = textureId;
}

void Button::Render()
{
    background_rectangle.projection = *mProjection;
    background_rectangle.view = *mView;
    background_rectangle.model = mModel;

    DE_drawRectangle(&background_rectangle);

    if (mText != "")
    {
        glm::mat4 txt_model = glm::translate(mModel, glm::vec3(0, -20, 0));

        textRenderer_v2->setCustomPV(*mProjection, *mView);
        textRenderer_v2->RenderText(1, mText, txt_model, TextRenderer_v2::TEXT_CENTER);
    }

    if ((isLockable == true) && (locked == true))
    {
        lock_rectangle.projection = *mProjection;
        lock_rectangle.view = *mView;
        lock_rectangle.model = glm::scale(glm::translate(mModel, glm::vec3(50, -45, 0)), glm::vec3(0.35, 0.35, 0.35));

        DE_drawRectangle(&lock_rectangle);
    }
}

bool Button::onPointerDown(float x_ndc, float y_ndc)
{
    cout << "Button::onPointerDown x = " << x_ndc << " y = " << y_ndc << endl;

    glm::vec3 window_position(x_ndc, mViewport->w - y_ndc, 0.0f);
    glm::vec3 world_position = glm::unProject(window_position, mModel * (*mView), *mProjection, *mViewport);

    cout << "world_position.x = " << world_position.x << " world_position.y = " << world_position.y << endl;

    if ((world_position.x >= (-mDimm.x / 2.0f)) && (world_position.x <= (mDimm.x / 2.0f)) && (world_position.y >= (-mDimm.y / 2.0f)) && (world_position.y <= (mDimm.y / 2.0f)))
    {
        isTouched = true;
        background_rectangle.texture_id = touchedTexture;
        return true;
    }
    return false;
}

void Button::onPointerUp()
{
    if (isTouched == true)
    {
        isTouched = false;
        background_rectangle.texture_id = normalTexture;
        if (mListener)
        {
            mListener->Button_onClicked(this);
        }
    }
}
