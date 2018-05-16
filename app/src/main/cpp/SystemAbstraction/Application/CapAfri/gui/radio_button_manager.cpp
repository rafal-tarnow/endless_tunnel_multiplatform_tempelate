#include "radio_button_manager.hpp"
#include <iostream>

using namespace std;

RadioButtonManager::RadioButtonManager()
{

}

RadioButtonManager::~RadioButtonManager()
{

}

void RadioButtonManager::addRadioButton(RadioButton * button)
{
    button->setEventListener(this);
    radioButtons.push_back(button);
}

void RadioButtonManager::Button_onClicked(Button * button)
{
    RadioButton * radioButton = dynamic_cast<RadioButton*>(button);

    RadioButton * tmpRadioButton = nullptr;

    for(unsigned int i = 0; i < radioButtons.size(); i++)
    {
        tmpRadioButton = radioButtons.at(i);

        if(tmpRadioButton == radioButton)
        {
            tmpRadioButton->setRadioState(true);
        }
        else
        {
            tmpRadioButton->setRadioState(false);
        }
    }


    cout << "RadioButtonManager::Button_onClicked(Button * button)" << endl;
}
