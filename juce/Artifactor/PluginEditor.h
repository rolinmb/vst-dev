#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

class ArtifactorAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    ArtifactorAudioProcessorEditor (ArtifactorAudioProcessor&);
    ~ArtifactorAudioProcessorEditor() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    juce::Slider mGainSlider;
    juce::Slider mRangeSlider;
    juce::Slider mBlendSlider;
    juce::Slider mVolumeSlider;

    juce::Label mGainLabel;
    juce::Label mRangeLabel;
    juce::Label mBlendLabel;
    juce::Label mVolumeLabel;

    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> gainAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> rangeAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> blendAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> volumeAttachment;

    void buildSliderWithAttachment(juce::AudioProcessorValueTreeState& state,
        juce::Slider& sldr, std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>& atchmnt, juce::Label& lbl,
        float minVal, float maxVal, float interVal, float defaultVal, juce::String name, juce::String labelText);

    juce::ComboBox fuzzSelector;

    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> selectAttachment;

    ArtifactorAudioProcessor& audioProcessor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ArtifactorAudioProcessorEditor)
};
