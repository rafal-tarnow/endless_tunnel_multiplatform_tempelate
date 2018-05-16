#include "radio_button.hpp"
#include <library_opengles_2/Resources/Resources.hpp>
#include <iostream>

using namespace std;

RadioButton::RadioButton()
{

}

RadioButton::~RadioButton()
{

}

void RadioButton::setRadioState(bool state)
{
    isRadioActive = state;
}

void RadioButton::Render()
{
    rectangle.projection = *mProjection;
    rectangle.view = *mView;
    rectangle.model = mModel;

    if(isRadioActive || isTouched)
    {
        rectangle.texture_id = touchedTexture;
    }
    else
    {
        rectangle.texture_id = normalTexture;
    }

    DE_drawRectangle(&rectangle);
}
