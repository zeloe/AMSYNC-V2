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
#include "Waveshapes.h"
class Sine_Wave
{
public:
    Sine_Wave(){};
    ~Sine_Wave() {};
    
    void prepare(const juce::dsp::ProcessSpec& spec)
    {
        m_sampleRate = spec.sampleRate;        
        phase = 0;
        temp_freq = 0;
    }
    void setFreq(float freq)
    {
        current_freq = freq;
    }
    
    float process_waveTable(float freq)
    {
        
        phase += size / (m_sampleRate / freq);

        if ( phase >= size - 1) phase -=size;
        remainder = phase - floor(phase);
        const float output = (float) ((1.0-remainder) * sineBuffer[1+ (long) phase] + remainder * sineBuffer[2+(long) phase]);
        return output;
    }
    
    void process(juce::dsp::AudioBlock<float> block) noexcept
    {
        //Mono
        auto inputBlock  =  block.getSingleChannelBlock(0);
        auto outputBlock =  block.getSingleChannelBlock(0);
        auto numSamples  =  block.getNumSamples();
         
        auto* input  = inputBlock .getChannelPointer (0);
        auto* output = outputBlock.getChannelPointer (0);
         
        if(temp_freq != current_freq)
        {
            // this works for block based processing
            freq_inc = (current_freq - temp_freq) / numSamples;
            for (size_t i = 0; i < numSamples; ++i)
            {
                temp_freq += freq_inc;
                const float ampmod = input[i] * process_waveTable(temp_freq);
                output[i] = ampmod;
            }
            temp_freq = current_freq;
        } else {
            for (size_t i = 0; i < numSamples; ++i)
            {
                const float ampmod = input[i] * process_waveTable(temp_freq);
                output[i] = ampmod;
            }
        }
             
    }
    float phase = 0;
private:
    float frequency;
    float temp_freq;
    float current_freq;
    float freq_inc = 0;
    float increment = 0;
    float m_sampleRate = -1;
   
    float remainder = 0;
    float size = 512;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Sine_Wave)
};
