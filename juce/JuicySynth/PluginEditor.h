/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "UI/AdsrComponent.h"
#include "UI/OscComponent.h"
#include "UI/FilterComponent.h"

//==============================================================================
/**
*/
class JuicySynthAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    JuicySynthAudioProcessorEditor (JuicySynthAudioProcessor&);
    ~JuicySynthAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    JuicySynthAudioProcessor& audioProcessor;

    OscComponent osc;
    AdsrComponent adsr;
    AdsrComponent filterAdsr;
    FilterComponent filter;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (JuicySynthAudioProcessorEditor)
};
