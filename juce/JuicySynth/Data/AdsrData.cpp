/*
  ==============================================================================

    AdsrData.cpp
    Created: 8 May 2023 7:56:09pm
    Author:  rolin

  ==============================================================================
*/

#include "AdsrData.h"

void AdsrData::update(const float a, const float d, const float s, const float r)
{
    adsrParams.attack = a;
    adsrParams.decay = d;
    adsrParams.sustain = s;
    adsrParams.release = r;

    setParameters(adsrParams);
}