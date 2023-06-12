/*
  ==============================================================================

    OscComponent.cpp
    Created: 8 May 2023 9:14:51pm
    Author:  rolin

  ==============================================================================
*/

#include <JuceHeader.h>
#include "OscComponent.h"

//==============================================================================
OscComponent::OscComponent(juce::AudioProcessorValueTreeState& apvts, juce::String waveSelectorId, juce::String fmFreqId, juce::String fmDepthId)
{
    juce::StringArray choices {"Sine", "Saw", "Square"};
    waveSelector.addItemList(choices, 1);
    addAndMakeVisible(waveSelector);

    waveAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(apvts, waveSelectorId, waveSelector);

    setSliderParams(apvts, fmFreqAttachment, fmFreqSlider, fmFreqId);
    setSliderParams(apvts, fmDepthAttachment, fmDepthSlider, fmDepthId);
}

OscComponent::~OscComponent()
{
}

void OscComponent::paint (juce::Graphics& g)
{
    auto bounds = getLocalBounds().reduced(5);
    auto labelSpace = bounds.removeFromTop(25.0f);

    g.fillAll(juce::Colours::black);
    g.setColour(juce::Colours::white);
    g.setFont(20.0f);
    g.drawText("Oscillator", labelSpace.withX(5), juce::Justification::left);
    g.drawRoundedRectangle(bounds.toFloat(), 5.0f, 2.0f);
}

void OscComponent::resized()
{
    const auto startY = 55;
    const auto sliderWidth = 100;
    const auto sliderHeight = 90;
    const auto labelYOffset = 20;
    const auto labelHeight = 20;

    waveSelector.setBounds(10, startY + 5, 90, 30);
    fmFreqSlider.setBounds(waveSelector.getRight(), startY, sliderWidth, sliderHeight);
    fmDepthSlider.setBounds(fmFreqSlider.getRight(), startY, sliderWidth, sliderHeight);
}

void OscComponent::setSliderParams(juce::AudioProcessorValueTreeState& treeState,
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>& attachment,
    juce::Slider& sldr, juce::String paramId)
{
    sldr.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    sldr.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 50, 25);
    addAndMakeVisible(sldr);

    attachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(treeState, paramId, sldr);
}
