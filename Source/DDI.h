/*
  ==============================================================================

    DDI.h
    Created: 29 Jun 2023 1:22:12pm
    Author:  Onez

  ==============================================================================
*/

#pragma once
#include "JuceHeader.h"
#include "bpm_to_samps.h"
#include "BlockInterpol.h"
class DDI
{
public:
    DDI()
    {
        DDI::delayBuf.resize(512);
        DDI::delayBuf2.resize(512);
        
    };
    ~DDI(){};
    
    void prepare(const juce::dsp::ProcessSpec& spec)
    {
        delayBuf.clear();
        delayBuf.resize(bpm_to_samps(2,2,spec.sampleRate));
        delayBuf2.clear();
        delayBuf2.resize(bpm_to_samps(2,2,spec.sampleRate));
        size = int(delayBuf.size());
        temp_delay = 0;
        current_delay = 0;
        fadeto1 = 0;
        fadeto2 = 0;
        m_sampleRate = spec.sampleRate;
        std::vector<float>::iterator ptr;
        writePointer = 0;
        writePointer2  = 0;
        inc_fade = (1.0) / (spec.maximumBlockSize);
        // initialize a vector with zeros (to save ear damage)
        for(ptr = delayBuf.begin(); ptr < delayBuf.end(); ptr++)
        {
            *ptr =0;
        }
        
        for(ptr = delayBuf2.begin(); ptr < delayBuf2.end(); ptr++)
        {
            *ptr =0;
        }
        
    }
    
    void setDelay(float delay)
    {
        float temp = (int)floor(delay);
        current_fract = delay - temp;
        current_delay = int(delay);
        if (state_int == 0)
        {
            state = true;
        }
        else
        {
            state = false;
        }
        state_int += 1;
        if (state_int > 1) state_int = 0;
    }
    
    void process(juce::dsp::AudioBlock<float> block) noexcept
    {
        // Mono
        auto inputBlock  = block.getSingleChannelBlock(0);
        auto outputBlock = block.getSingleChannelBlock(0);
        auto numSamples  = block.getNumSamples();
        
        auto* input  = inputBlock.getChannelPointer(0);
        auto* output = outputBlock.getChannelPointer(0);
        //delay time changes
        if (temp_delay != current_delay)
        {
            fract_inc = (current_fract - fract) / numSamples;
            if (state == true)
            {
                //reset
                
                fadeto2 = 0;
                fadeto1 = 1;
                for(size_t i = 0; i < numSamples; i++)
                {
                    
                    fract += fract_inc;
                    readPointer = writePointer - temp_delay;
                    //update delay line 2
                    readPointer2 = writePointer - current_delay;
                    
                   
                    //check if above size
                    if (writePointer >= size) {
                        writePointer = 0;
                    }
                    
                   
                    
                    
                    
                    //write to DelayLines
                    delayBuf[writePointer] = input[i];
                    delayBuf2[writePointer] = input[i];
                    
                    if (readPointer - 1 < 0) {
                        readPointer += size;
                    }
                    
                    if (readPointer2 - 1 < 0) {
                        readPointer2 += size;
                    }
                   
                   
                    // get current Sample of delayLine
                    fadeto2 += inc_fade;
                    const float x0 = delayBuf[(readPointer -1 + size) % size];
                    const float x1 = delayBuf[(readPointer + size) % size];
                    const float y0 = delayBuf2[(readPointer2 -1 + size) % size];
                    const float y1 = delayBuf2[(readPointer2 + size) % size];
                 
                    const float x_est = linear_interp(x0, x1, fract);
                    const float y_est = linear_interp(y0, y1, fract);
                    
                    output[i] = linear_interp(x_est, y_est, fadeto2);
                    
    
                    //update
                    writePointer++;
                   
                   
                    
                }
                temp_delay = current_delay;
                fract = current_fract;
                fadeto2 = 1;
            }
            else
            {
                
                //reset
                fadeto1 = 0;
                fadeto2 = 1;
                for(size_t i = 0; i < numSamples; i++)
                {
                    fract += fract_inc;
                    readPointer = writePointer - current_delay;
                    readPointer2 = writePointer - temp_delay;
                    
                    if (writePointer >= size) {
                        writePointer = 0;
                    }
                    
                    
                    
                    //write to DelayLines
                    delayBuf[writePointer] = input[i];
                    delayBuf2[writePointer] = input[i];
                    
                    
                    if (readPointer - 1 < 0) {
                        readPointer += size;
                    }
                    
                    if (readPointer2 - 1 < 0) {
                        readPointer2 += size;
                    }
                   
                   
                    // get current Sample of delayLine
                    fadeto1 += inc_fade;
                    const float x0 = delayBuf[(readPointer -1 + size) % size];
                    const float x1 = delayBuf[(readPointer + size) % size];
                    const float y0 = delayBuf2[(readPointer2 -1 + size) % size];
                    const float y1 = delayBuf2[(readPointer2 + size) % size];
                 
                    const float x_est = linear_interp(x0, x1, fract);
                    const float y_est = linear_interp(y0, y1, fract);
                    
                    
                    
                    output[i] = linear_interp(y_est, x_est,fadeto1);
                    
                        
                    
                    //update
                    writePointer++;
                    
                    
                    
                }
                temp_delay = current_delay;
                fract = current_fract;
                fadeto1 = 1;
                fadeto2 = 0;
            }
        }
        else
        {
            for (size_t i = 0; i < numSamples; ++i)
            {
                if (state == true)
                {
                    fadeto1 = 1.0;
                    fadeto2 = 0.0;
                    readPointer = writePointer - (current_delay);
                }
                else
                {
                    fadeto1 = 0.0;
                    fadeto2 = 1.0;
                    readPointer2 = writePointer  - (current_delay);
                }
                
                delayBuf[writePointer] = input[i];
                delayBuf2[writePointer] = input[i];
                writePointer = (writePointer + 1) % size;
                
                const float x0 = delayBuf[(readPointer + size) % size];
                const float y0 = delayBuf2[(readPointer2 + size) % size];
                
                  
                output[i] = x0 * fadeto1 + y0 * fadeto2;
            }
        }
    }



    
   
    
private:
    std::vector<float> delayBuf;
    std::vector<float> delayBuf2;
    unsigned int size = 0;
    unsigned int readPointer = 0;
    unsigned int writePointer = 0;
    unsigned int readPointer2 = 0;
    unsigned int writePointer2 = 0;
    int delay1 = 0;
    int delay2 = 0;
    float temp_delay = 0;
    float delay_inc = 0;
    float current_delay = 0;
    float current_fract = 0;
    float inc_fract_1 = 0;
    float inc_fract_2 = 0;
    float fract_inc = 0;
    float fract_1 = 0;
    float fract = 0;
    float temp_fract = 0;
    float temp_fract2 = 0;
    float m_sampleRate = 0;
    float nv;
    float inc_fade = 0;
    float fadeto1 = 0;
    float fadeto2 = 0;
    bool switcher = true;
    bool state = true;
    int state_int = 0;
    float x0,x1,x2,x3;
    float y0,y1,y2,y3;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DDI)
};
