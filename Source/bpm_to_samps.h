/*
  ==============================================================================

    bpm_to_samps.h
    Created: 20 Jan 2023 3:09:35pm
    Author:  Onez

  ==============================================================================
*/

#pragma once
inline float bpm_to_samps ( const float  bpm,const int timesignatureindex, const float SR)
{
    float result;
        switch (timesignatureindex) {
            case 0:
                result =  4.0; // 1/4 note
                break;
            case 1:
                result =  8.0; // 1/8 note
                break;
            case 2:
                result =  16.0; // 1/16 note
                break;
            case 3:
                result = 32.0; // 1/32 note
                break;
            case 4:
                result =  64.0; // 1/64 note
                break;
            case 5:
                result =  128.0; // 1/128 note
                break;
            default:
                result = 4.0; // 1/4 note
                break;
        }
        return ((60.0 / bpm) / result) * SR;
}

inline float bpm_to_s ( const float  bpm,const int timesignatureindex)
{
    
    float result;
        switch (timesignatureindex) {
            case 0:
                result =  4.0; // 1/4 note
                break;
            case 1:
                result =  8.0; // 1/8 note
                break;
            case 2:
                result =  16.0; // 1/16 note
                break;
            case 3:
                result = 32.0; // 1/32 note
                break;
            case 4:
                result =  64.0; // 1/64 note
                break;
            case 5:
                result =  128.0; // 1/128 note
                break;
            default:
                result = 4.0; // 1/4 note
                break;
        }
        return ((60.0 / bpm) / result);
}
