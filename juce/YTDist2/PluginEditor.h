#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
class YTDist2AudioProcessorEditor  : public juce::AudioProcessorEditor,
                                     public juce::Slider::Listener
{
public:
    YTDist2AudioProcessorEditor (YTDist2AudioProcessor&);
    ~YTDist2AudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;
    // implementing Slider::Listener, method may be deprecated but still necessary to implement as void
    void sliderValueChanged(juce::Slider* slider) override;

private:    
    YTDist2AudioProcessor& audioProcessor;
    juce::Slider driveKnob;
    juce::Slider rangeKnob;
    juce::Slider blendKnob;
    juce::Slider volumeKnob;

public:
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> driveSliderVal;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> rangeSliderVal;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> blendSliderVal;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> volumeSliderVal;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (YTDist2AudioProcessorEditor)
};
