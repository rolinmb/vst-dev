#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class JuceDelayAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    JuceDelayAudioProcessorEditor (JuceDelayAudioProcessor&);
    ~JuceDelayAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    juce::Slider mDelaySlider;

    juce::Label mDelayLabel;

    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> delayAttachment;

    JuceDelayAudioProcessor& audioProcessor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (JuceDelayAudioProcessorEditor)
};
