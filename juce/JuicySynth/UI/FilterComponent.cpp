/*
  ==============================================================================

    FilterComponent.cpp
    Created: 8 May 2023 10:50:14pm
    Author:  rolin

  ==============================================================================
*/

#include <JuceHeader.h>
#include "FilterComponent.h"

//==============================================================================
FilterComponent::FilterComponent(juce::AudioProcessorValueTreeState& apvts, juce::String filterTypeSelectorId, juce::String filterFreqId, juce::String filterResId)
{
    juce::StringArray choices { "Low-Pass", "Band-Pass", "High-Pass"};
    filterTypeSelector.addItemList(choices, 1);
    addAndMakeVisible(filterTypeSelector);

    filterTypeSelectorAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(apvts, filterTypeSelectorId, filterTypeSelector);

    setSliderParams(apvts, filterFreqAttachment, filterFreqSlider, filterFreqId);
    setSliderParams(apvts, filterResAttachment, filterResSlider,filterResId);
}

FilterComponent::~FilterComponent()
{
}

void FilterComponent::paint (juce::Graphics& g)
{
    auto bounds = getLocalBounds().reduced(5);
    auto labelSpace = bounds.removeFromTop(25.0f);

    g.fillAll(juce::Colours::black);
    g.setColour(juce::Colours::white);
    g.setFont(20.0f);
    g.drawText("Filter", labelSpace.withX(5), juce::Justification::left);
    g.drawRoundedRectangle(bounds.toFloat(), 5.0f, 2.0f);
}

void FilterComponent::resized()
{
    const auto startY = 55;
    const auto sliderWidth = 100;
    const auto sliderHeight = 90;
    const auto labelYOffset = 20;
    const auto labelHeight = 20;

    filterTypeSelector.setBounds(10, startY + 5, 90, 30);
    filterFreqSlider.setBounds(filterTypeSelector.getRight(), startY, sliderWidth, sliderHeight);
    filterResSlider.setBounds(filterFreqSlider.getRight(), startY, sliderWidth, sliderHeight);
}

void FilterComponent::setSliderParams(juce::AudioProcessorValueTreeState& treeState,
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>& attachment,
    juce::Slider& sldr, juce::String paramId)
{
    sldr.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    sldr.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 50, 25);
    addAndMakeVisible(sldr);

    attachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(treeState, paramId, sldr);
}
