/*
  ==============================================================================

    Sliders_Custom.h
    Created: 7 Feb 2023 4:40:48pm
    Author:  Onez

  ==============================================================================
*/

#pragma once
#include "Slider_Custom.h"
class Zen_Slider : public juce::Slider
{
public:
    Zen_Slider(juce::String Tooltip)
    {
        this->setTooltip(Tooltip);
    };
    
    ~Zen_Slider() override {
        setLookAndFeel(nullptr);
    };
    
    enum class ZenStyles
        {
          greenslider,
          redslider
        };
    void setStyles(ZenStyles styles)
    {
        switch (styles) {
            case ZenStyles::greenslider:
                setLookAndFeel(&GreenStyle);
                break;
            case ZenStyles::redslider:
                setLookAndFeel(&RedStyle);
                break;
        }
    }
private:
    juce::String tooltip;
    juce::Zen_GreenSlider GreenStyle;
    juce::Zen_RedSlider RedStyle;
    
    
};
