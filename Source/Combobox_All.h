/*
  ==============================================================================

    Combox_All.h
    Created: 7 Feb 2023 5:37:12pm
    Author:  Onez

  ==============================================================================
*/

#pragma once
#include "Combobox_Custom.h"
class Zen_Combobox : public juce::ComboBox
{
public:
    Zen_Combobox()  {};
    
    ~Zen_Combobox()  {
        setLookAndFeel(nullptr);
    };
    
    enum class ZenStyles
        {
          greenbox,
          redbox
        };
    void setStyles(ZenStyles styles)
    {
        switch (styles) {
            case ZenStyles::greenbox:
                setLookAndFeel(&greenComboBox);
                break;
            case ZenStyles::redbox:
               setLookAndFeel(&redComboBox);
                break;
        }
    }
private:
    
    juce::Zen_GreenComboBox greenComboBox;
    juce::Zen_RedComboBox redComboBox;
    
    
};
