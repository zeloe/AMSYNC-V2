/*
  ==============================================================================

    BlockInterpol.h
    Created: 5 Feb 2023 3:11:35pm
    Author:  Onez

  ==============================================================================
*/

#pragma once
#include "JuceHeader.h"

inline float linear_interp(float const v0, float const v1, float const t )
{
 //   return (1 - t) * v0 + t * v1;
    return v0 + t*(v1-v0);
}


inline float cubicInterpolation(float y0, float y1, float y2, float y3, float x)
{
        const float a0 = y3 - y2 - y0 + y1;
        const float a1 = y0 - y1 - a0;
        const float a2 = y2 - y0;
        const float a3 = y1;
        return a0 * x * x * x + a1 * x * x + a2 * x + a3;
    
}
