#pragma once

#include "radio_button.hpp"
#include "radio_button_manager.hpp"
#include <vector>

using namespace std;

class RadioButtonManagerEventLister
{
  public:
    virtual void RadioButtonManager_onRadioButtonChanged(RadioButton * radioButton) = 0;
};


class RadioButtonManager : public ButtonEventListener{
public:
    RadioButtonManager();
    ~RadioButtonManager();

    void addRadioButton(RadioButton * button);

    void setEventListener(RadioButtonManagerEventLister *listener);

    void Button_onClicked(Button * button);

private:
    RadioButtonManagerEventLister * mLister = nullptr;
    vector<RadioButton *> radioButtons;

};
