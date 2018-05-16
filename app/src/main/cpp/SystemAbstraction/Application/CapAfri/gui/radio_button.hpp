#pragma once

#include <library_opengles_2/RectangleRenderer/Rectangle_Renderer.hpp>
#include <string>
#include "button.hpp"

using namespace std;

class RadioButton : public Button{
public:
    RadioButton();
    ~RadioButton();

    void setRadioState(bool state);

     virtual void Render();
private:
    bool isRadioActive = false;
};
