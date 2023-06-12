#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

class BiccrushAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    BiccrushAudioProcessorEditor (BiccrushAudioProcessor&);
    ~BiccrushAudioProcessorEditor() override;

    void paint (juce::Graphics&) override;
    void resized() override;

    juce::Slider bitsRedux;
    juce::Slider rateRedux;
    juce::Slider noise;

    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> bitsAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> rateAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> noiseAttachment;

private:

    BiccrushAudioProcessor& audioProcessor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (BiccrushAudioProcessorEditor)
};
