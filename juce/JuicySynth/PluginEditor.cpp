/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
JuicySynthAudioProcessorEditor::JuicySynthAudioProcessorEditor (JuicySynthAudioProcessor& p)
    : AudioProcessorEditor (&p)
    , audioProcessor (p)
    , osc(audioProcessor.apvts, "WAVEFORM", "FM FREQUENCY", "FM DEPTH")
    , adsr(audioProcessor.apvts, "ATTACK", "DECAY", "SUSTAIN", "RELEASE")
    , filterAdsr(audioProcessor.apvts, "FILTER ATTACK", "FILTER DECAY", "FILTER SUSTAIN", "FILTER RELEASE")
    , filter(audioProcessor.apvts, "FILTER TYPE", "FILTER CUTOFF FREQUENCY", "FILTER RESONANCE")
{
    setSize (620, 500);

    addAndMakeVisible(osc);
    addAndMakeVisible(adsr);
    addAndMakeVisible(filterAdsr);
    addAndMakeVisible(filter);
}

JuicySynthAudioProcessorEditor::~JuicySynthAudioProcessorEditor()
{
}

//==============================================================================
void JuicySynthAudioProcessorEditor::paint (juce::Graphics& g)
{
    g.fillAll(juce::Colours::black);
}

void JuicySynthAudioProcessorEditor::resized()
{
    const auto paddingX = 5;
    const auto paddingY = 35;
    const auto paddingY2 = 235;

    osc.setBounds(paddingX, paddingY, 300, 200);
    adsr.setBounds(osc.getRight(), paddingY, 300, 200);
    filterAdsr.setBounds(paddingX, paddingY2, 300, 200);
    filter.setBounds(filterAdsr.getRight(), paddingY2, 300, 200);
}