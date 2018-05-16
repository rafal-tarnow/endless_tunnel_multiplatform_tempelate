#pragma once

#include "radio_button.hpp"
#include "radio_button_manager.hpp"
#include <vector>

using namespace std;

class RadioButtonManager : public ButtonEventListener{
public:
    RadioButtonManager();
    ~RadioButtonManager();

    void addRadioButton(RadioButton * button);

    void Button_onClicked(Button * button);

private:
    vector<RadioButton *> radioButtons;

};
