/*
  ==============================================================================

    OscData.h
    Created: 8 May 2023 8:43:52pm
    Author:  rolin

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

class OscData : public juce::dsp::Oscillator<float>
{
public:
    void prepareToPlay(juce::dsp::ProcessSpec& spec);
    void setWaveType(const int choice);
    void setWaveFrequency(const int midiNoteNumber);
    void getNextAudioBlock(juce::dsp::AudioBlock<float>& block);
    void updateFm(const float freq, const float depth);

private:
    void processFmOsc(juce::dsp::AudioBlock<float>& block);

    juce::dsp::Oscillator<float> fmOsc { [](float x) { return std::sin(x); } };
    float fmMod { 0.0f };
    float fmDepth { 0.0f };
    int lastMidiNote { 0 };
};