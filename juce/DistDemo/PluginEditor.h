/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class DistDemoAudioProcessorEditor  : public juce::AudioProcessorEditor/*,
                                      public juce::Slider::Listener*/
{
public:
    DistDemoAudioProcessorEditor (DistDemoAudioProcessor&);
    ~DistDemoAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;
    // implementing Slider::Listener
    //void sliderValueChanged(juce::Slider* slider) override;

private:
    
    juce::Slider mGainSlider;

    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> gainAttachment;

    DistDemoAudioProcessor& audioProcessor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DistDemoAudioProcessorEditor)
};
