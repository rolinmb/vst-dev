/*
  ==============================================================================

    SynthSound.h
    Created: 7 May 2023 8:11:57pm
    Author:  rolin

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class SynthSound : public juce::SynthesiserSound
{
public:
    bool appliesToNote (int midiNoteNumber) override { return true; };
    bool appliesToChannel (int midiChannel) override { return true; };
};