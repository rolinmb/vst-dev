#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class YoutubeDistortionAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    YoutubeDistortionAudioProcessorEditor (YoutubeDistortionAudioProcessor&);
    ~YoutubeDistortionAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    // define GUI sliders
    juce::Slider mDriveSlider;
    juce::Slider mRangeSlider;
    juce::Slider mBlendSlider;
    juce::Slider mVolumeSlider;

    juce::Label mDriveLabel;
    juce::Label mRangeLabel;
    juce::Label mBlendLabel;
    juce::Label mVolumeLabel;

    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> driveAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> rangeAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> blendAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> volumeAttachment;

    YoutubeDistortionAudioProcessor& audioProcessor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (YoutubeDistortionAudioProcessorEditor)
};
