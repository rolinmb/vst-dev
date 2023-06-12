/*
  ==============================================================================

    AdsrData.h
    Created: 8 May 2023 7:56:09pm
    Author:  rolin

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class AdsrData  : public juce::ADSR
{
public:
    void update(const float a, const float d, const float s, const float r);

private:
    juce::ADSR::Parameters adsrParams;
};
