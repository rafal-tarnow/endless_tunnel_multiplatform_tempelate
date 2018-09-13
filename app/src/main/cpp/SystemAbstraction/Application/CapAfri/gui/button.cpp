#include "button.hpp"
#include <library_opengles_2/Resources/Resources.hpp>
#include <iostream>

using namespace std;

Button::Button() : ObjectCounter("Button")
{ 
    mDimm = glm::vec2(246,133);

    DE_initRectangle_4(&rectangle,normalTexture,mDimm);


}

Button::~Button()
{
    DE_deleteRectangle(&rectangle);
    if(textRenderer_v2)
        delete textRenderer_v2;
}

void Button::setText(string text)
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

string Button::getText()
{
    return mText;
}

void Button::setPosition(glm::vec3 &position)
{
    mPosition = position;
    mModel = glm::translate(glm::mat4(1),position);
}

void Button::setModel(glm::mat4 model)
{
    mModel = model;
}

void Button::setDimm(glm::vec2 dim)
{
    mDimm = dim;
    DE_setDimm(&rectangle, mDimm);
}

void Button::setPressed(bool pressed)
{
    if(pressed == true)
    {
        isTouched = true;
        rectangle.texture_id = touchedTexture;
    }else
    {
        isTouched = false;
        rectangle.texture_id = normalTexture;
    }

}

void Button::setMatrices(glm::vec4 *Viewport, glm::mat4 *Projection, glm::mat4 *View)
{
    mViewport = Viewport;
    mProjection = Projection;
    mView = View;
}

void Button::setEventListener(ButtonEventListener * listener)
{
    mListener = listener;
}

void Button::setNormalBackgroundTexture(GLuint textureId)
{
    normalTexture = textureId;
    if(isTouched == false)
    {
        rectangle.texture_id = normalTexture;
    }
}

void Button::setPressedBackgroundTexture(GLuint textureId)
{
    touchedTexture = textureId;
    if(isTouched == true)
    {
        rectangle.texture_id = touchedTexture;
    }
}

void Button::Render()
{
    rectangle.projection = *mProjection;
    rectangle.view = *mView;
    rectangle.model = mModel;

    DE_drawRectangle(&rectangle);


    if(mText != "")
    {
        glm::mat4 txt_model = glm::translate(mModel, glm::vec3(0,-20,0));


        textRenderer_v2->setCustomPV(*mProjection, *mView);
        textRenderer_v2->RenderText(1,mText, txt_model, TextRenderer_v2::TEXT_CENTER);
    }
}

bool Button::onPointerDown(float x_ndc, float y_ndc)
{
    cout << "Button::onPointerDown x = " << x_ndc << " y = " <<  y_ndc << endl;

    glm::vec3 window_position(x_ndc, mViewport->w - y_ndc, 0.0f);
    glm::vec3 world_position = glm::unProject(window_position,mModel*(*mView), *mProjection, *mViewport);

    cout << "world_position.x = " << world_position.x << " world_position.y = " << world_position.y << endl;

    if((world_position.x >= (-mDimm.x/2.0f)) && (world_position.x <= (mDimm.x/2.0f)) && (world_position.y >= (-mDimm.y/2.0f)) && (world_position.y <= (mDimm.y/2.0f)))
    {
        isTouched = true;
        rectangle.texture_id = touchedTexture;
        return true;
    }
    return false;
}

void Button::onPointerUp()
{
    if(isTouched == true)
    {
        isTouched = false;
        rectangle.texture_id = normalTexture;
        if(mListener)
        {
            mListener->Button_onClicked(this);
        }
    }
}
