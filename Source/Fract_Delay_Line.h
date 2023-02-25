/*
  ==============================================================================

    Fract_Delay_Line.h
    Created: 6 Feb 2023 1:57:36pm
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
#include "bpm_to_samps.h"
#include "BlockInterpol.h"
class Fract_delay_line
{
public:
    Fract_delay_line()
    {
        Fract_delay_line::delayBuf.resize(512);
    };
    ~Fract_delay_line() {};
    
    void prepare(const juce::dsp::ProcessSpec& spec)
    {
        delayBuf.clear();
        delayBuf.resize(bpm_to_samps(2,2,spec.sampleRate));
        size = int(delayBuf.size());
        readPointer = 0;
        writePointer = 0;
        temp_delay = 0;
        current_delay = 0;
        m_sampleRate = spec.sampleRate;
        std::vector<float>::iterator ptr;
        // initialize a vector with zeros (to save ear damage)
        for(ptr = delayBuf.begin(); ptr < delayBuf.end(); ptr++)
        {
            *ptr =0;
        }
        
        
        
    }
    void setDelay(float delay)
    {
        float temp = (int)floor(delay);
        current_fract = delay - temp;
        current_delay = int(delay);
    }
    
    void process(juce::dsp::AudioBlock<float> block) noexcept
    {
        //Mono
        auto inputBlock  = block.getSingleChannelBlock(0);
        auto outputBlock = block.getSingleChannelBlock(0);
         auto numSamples  = block.getNumSamples();
         
         auto* input  = inputBlock .getChannelPointer (0);
         auto* output = outputBlock.getChannelPointer (0);
         
    if (temp_delay != current_delay) {
        // Compute the increment of the fractional delay in each block
        delay_inc = (current_delay - temp_delay) / numSamples;
        fract_inc = (current_fract - temp_fract) / numSamples;
        for (size_t i = 0; i < numSamples; ++i) {
            temp_delay += delay_inc;
            temp_fract += fract_inc;
            readPointer = writePointer - temp_delay;
            delayBuf[writePointer] = input[i];
            if (readPointer - 3 < 0) {
                readPointer += size;
            }
            const float y0 = delayBuf[(readPointer - 3) % size];
            const float y1 = delayBuf[(readPointer - 2) % size];
            const float y2 = delayBuf[(readPointer - 1) % size];
            const float y3 = delayBuf[readPointer % size];
            const float x_est = cubicInterpolation(y0, y1, y2, y3, temp_fract);
            writePointer++;
            if (writePointer >= size) {
                writePointer = 0;
            }
            output[i] = x_est;
        }
        temp_delay = current_delay;
        temp_fract = current_fract;
    }   else {
        for (size_t i = 0; i < numSamples; ++i)
        {
            delayBuf[writePointer] = input[i];
            writePointer = (writePointer + 1) % size;
            readPointer = (writePointer - int(temp_delay) - 1 + size) % size;
            const float y0 = delayBuf[readPointer];
            output[i] = y0;
        }
    }
         
}
    
    
    
    
private:
    std::vector<float> delayBuf;
    float temp_delay = 0;
    float current_delay = 0;
    float temp_fract;
    float delay_inc;
    float current_fract;
    float fract_inc;
    float increment = 0;
    float m_sampleRate = -1;
    unsigned int readPointer = 0;
    unsigned int writePointer = 0;
    int size = 0;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Fract_delay_line)
};
