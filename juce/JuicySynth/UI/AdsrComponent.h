/*
  ==============================================================================

    AdsrComponent.h
    Created: 8 May 2023 7:55:35pm
    Author:  rolin

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class AdsrComponent  : public juce::Component
{
public:
    AdsrComponent(juce::AudioProcessorValueTreeState& apvts, juce::String aId, juce::String dId, juce::String sId, juce::String rId);
    ~AdsrComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    void setSliderParams(juce::AudioProcessorValueTreeState& treeState,
        std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>& attachment,
        juce::Slider& sldr, juce::String paramId);

    juce::Slider attackSlider;
    juce::Slider decaySlider;
    juce::Slider sustainSlider;
    juce::Slider releaseSlider;

    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> aAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> dAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> sAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> rAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AdsrComponent)
};
