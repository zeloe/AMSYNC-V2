/*
  ==============================================================================

    gain_block.h
    Created: 5 Feb 2023 11:39:34pm
    Author:  Onez

  ==============================================================================
*/

#pragma once
/*
  ==============================================================================

    Sine_Wave.h
    Created: 17 Jan 2023 5:37:34pm
    Author:  Onez

  ==============================================================================
*/

#pragma once
#include "JuceHeader.h"
#include "math.h"
class Gain_Block
{
public:
    Gain_Block()
    {
    };
    ~Gain_Block() {};
    
    void prepare()
    {
        temp_gain = 0;
    }
    
    void setGain(float gain)
    {
        current_gain = gain;
    }
    
    void process(juce::dsp::AudioBlock<float> block) noexcept
    {
        //Mono
        auto inputBlock  = block.getSingleChannelBlock(0);
        auto outputBlock = block.getSingleChannelBlock(0);
         auto numSamples  = block.getNumSamples();
         
         auto* input  = inputBlock .getChannelPointer (0);
         auto* output = outputBlock.getChannelPointer (0);
         
        if(temp_gain != current_gain)
        {
            // this works for block based processing
            gain_inc = (current_gain - temp_gain) / numSamples;
            for (size_t i = 0; i < numSamples; ++i)
            {
                temp_gain += gain_inc;
                const float gain = input[i] * temp_gain;
                output[i] = gain;
            }
            temp_gain = current_gain;
        } else {
            for (size_t i = 0; i < numSamples; ++i)
            {
                const float gain = input[i] * temp_gain;
                output[i] = gain;
            }
        }
             
    }
private:
    float temp_gain;
    float current_gain;
    float gain_inc = 0;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Gain_Block)
};
