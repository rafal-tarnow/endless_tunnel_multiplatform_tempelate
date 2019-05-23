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
//    rectangle.projection = *mProjection;
//    rectangle.view = *mView;
//    rectangle.model = mModel;

    if(isRadioActive || isTouched)
    {
        background_rectangle.texture_id = touchedTexture;
    }
    else
    {
        background_rectangle.texture_id = normalTexture;
    }

    Button::Render();

    //DE_drawRectangle(&rectangle);
}
