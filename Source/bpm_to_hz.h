/*
  ==============================================================================

    bpm_to_hz.h
    Created: 18 Jan 2023 12:21:35pm
    Author:  Onez

  ==============================================================================
*/

#pragma once
inline float bpm_to_herz (const float  bpm,const int  timesignatureindex)
{
    float beatLength;

        switch (timesignatureindex) {
            case 0:  // 1/4
                beatLength = 1.0 / 4.0;
                break;
            case 1:  // 1/8
                beatLength = 1.0 / 8.0;
                break;
            case 2:  // 1/16
                beatLength = 1.0 / 16.0;
                break;
            case 3:  // 1/32
                beatLength = 1.0 / 32.0;
                break;
            case 4:  // 1/64
                beatLength = 1.0 / 64.0;
                break;
            case 5:  // 1/128
                beatLength = 1.0 / 128.0;
                break;
            // Add more cases for different time signatures as needed
            default:
                // Use a default value of 1/4
                beatLength = 1.0 / 4.0;
                break;
        }

        // Convert beat length to seconds
        beatLength = beatLength / bpm * 60.0;

        // Compute the frequency of the sine wave
        return 1.0 / beatLength;
}
