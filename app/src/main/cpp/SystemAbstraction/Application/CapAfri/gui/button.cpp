#include "button.hpp"
#include <library_opengles_2/Resources/Resources.hpp>
#include <iostream>

using namespace std;

Button::Button()
{ 
    mDimm = glm::vec2(246,133);

    DE_initRectangle(&rectangle,normalTexture,mPosition,mDimm);
}

Button::~Button()
{
    DE_deleteRectangle(&rectangle);
}

void Button::setPosition(glm::vec3 &position)
{
    mPosition = position;
    mModel = glm::translate(glm::mat4(1),position);
}

void Button::setDimm(glm::vec2 dim)
{
    mDimm = dim;
    DE_setDimm(&rectangle, mDimm);
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
