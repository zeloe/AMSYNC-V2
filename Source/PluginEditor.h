/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "Parameters.h"
#include "Sliders_All.h"
#include "Combobox_All.h"
//==============================================================================
/**
*/
class AMSYNCAudioProcessorEditor  : public juce::AudioProcessorEditor//public juce::Timer
{
public:
    AMSYNCAudioProcessorEditor (AMSYNCAudioProcessor&);
    ~AMSYNCAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    AMSYNCAudioProcessor& audioProcessor;
    Zen_Slider Interpol_Delay_Left_Slider, Interpol_AmpModFreq_Left_Slider,  Mix_Delay_Left_Slider, Mix_Amp_Mod_Left_Slider;
    Zen_Slider Interpol_Delay_Right_Slider,
     Interpol_AmpModFreq_Right_Slider,
    Mix_Delay_Right_Slider,
    Mix_Amp_Mod_Right_Slider;
    Zen_Combobox DelayTimeLeft1, DelayTimeLeft2, AmpModLeft1, AmpModLeft2;
    Zen_Combobox   DelayTimeRight1, DelayTimeRight2,  AmpModRight1, AmpModRight2;
    juce::AudioProcessorValueTreeState::SliderAttachment Interpol_Delay_Left_Attach, Interpol_Delay_Right_Attach,
    Interpol_AmpModFreq_Left_Attach, Interpol_AmpModFreq_Right_Attach,
    Mix_Delay_Left_Attach,Mix_Delay_Right_Attach, Mix_Amp_Mod_Left_Attach,
    Mix_Amp_Mod_Right_Attach;
    juce::AudioProcessorValueTreeState::ComboBoxAttachment
    DelayTimeLeft1_Attach, DelayTimeRight1_Attach,  AmpModLeft1_Attach,AmpModRight1_Attach,
    DelayTimeLeft2_Attach,
    DelayTimeRight2_Attach,
    AmpModLeft2_Attach,
     AmpModRight2_Attach;
    juce::TooltipWindow tooltip;
    juce::Label delayInterpolTextButton, ampModInterpolTextButton, delayInterpolTextButtonR, ampModInterpolTextButtonR;
    juce::Label delayMixTextButton, ampModMixTextButton, delayMixTextButtonR, ampModMixTextButtonR;
    /*
    void timerCallback() override;
    void mouseEnter(const juce::MouseEvent &event) override;
    void mouseExit(const juce::MouseEvent &event) override;
    */
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AMSYNCAudioProcessorEditor)
};
