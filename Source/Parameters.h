/*
  ==============================================================================

    Parameters.h
    Created: 5 Feb 2023 2:27:48pm
    Author:  Onez

  ==============================================================================
*/

#pragma once
#include "JuceHeader.h"


const std::vector<juce::String> floatParams {
    "Interpol_Delay_Left",
    "Interpol_Delay_Right",
    "Interpol_AmpModFreq_Left",
    "Interpol_AmpModFreq_Right",
    "MixDelayL",
    "MixDelayR",
    "MixAmpModL",
    "MixAmpModR"
};

const std::vector<juce::String> choiceParams {
    "Delay_Time_Left_1",
    "Delay_Time_Right_1",
    "AmpMod_Freq_Left_1",
    "AmpMod_Freq_Right_1",
    "Delay_Time_Left_2",
    "Delay_Time_Right_2",
    "AmpMod_Freq_Left_2",
    "AmpMod_Freq_Right_2"
};

const juce::StringArray Times {
    "1/4",
    "1/8",
    "1/16",
    "1/32",
    "1/64",
    "1/128"
};



