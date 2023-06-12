/*
  ==============================================================================

    OscData.cpp
    Created: 8 May 2023 8:43:52pm
    Author:  rolin

  ==============================================================================
*/

#include "OscData.h"

void OscData::prepareToPlay(juce::dsp::ProcessSpec& spec)
{
    prepare(spec);
    fmOsc.prepare(spec);
}

void OscData::setWaveType(const int choice)
{
    switch (choice)
    {
    case 0:
        initialise([](float x) { return std::sin(x); }); // sine wave
        break;

    case 1:
        initialise([](float x) { return x / juce::MathConstants<float>::pi; }); // saw wave
        break;

    case 2:
        initialise([](float x) { return x < 0.0f ? -1.0f : 1.0f; }); // square wave
        break;
    
    default:
        jassertfalse;
        break;
    }
}

void OscData::setWaveFrequency(const int midiNoteNumber)
{
    setFrequency(juce::MidiMessage::getMidiNoteInHertz(midiNoteNumber) + fmMod);
    lastMidiNote = midiNoteNumber;
}

void OscData::getNextAudioBlock(juce::dsp::AudioBlock<float>& block)
{
    processFmOsc(block);
    process(juce::dsp::ProcessContextReplacing<float>(block));
}

void OscData::processFmOsc(juce::dsp::AudioBlock<float>& block)
{
    for (int ch = 0; ch < block.getNumChannels(); ++ch)
    {
        for (int s = 0; s < block.getNumSamples(); ++s)
        {
            fmMod = fmOsc.processSample(block.getSample(ch,s)) * fmDepth;
        }
    }
}

void OscData::updateFm(const float freq, const float depth)
{
    fmOsc.setFrequency(freq);
    fmDepth = depth;
    auto currentFreq = juce::MidiMessage::getMidiNoteInHertz(lastMidiNote) + fmMod;
    setFrequency(currentFreq >= 0 ? currentFreq : currentFreq * -1.0f);
}