/*
  ==============================================================================

    AdsrComponent.cpp
    Created: 8 May 2023 7:55:35pm
    Author:  rolin

  ==============================================================================
*/

#include <JuceHeader.h>
#include "AdsrComponent.h"

//==============================================================================
AdsrComponent::AdsrComponent(juce::AudioProcessorValueTreeState& apvts, juce::String aId, juce::String dId, juce::String sId, juce::String rId)
{
    setSliderParams(apvts, aAttachment, attackSlider, aId);
    setSliderParams(apvts, dAttachment, decaySlider, dId);
    setSliderParams(apvts, sAttachment, sustainSlider, sId);
    setSliderParams(apvts, rAttachment, releaseSlider, rId);
}

AdsrComponent::~AdsrComponent()
{
}

void AdsrComponent::paint (juce::Graphics& g)
{
    g.fillAll(juce::Colours::black);
}

void AdsrComponent::resized()
{
    const auto bounds = getLocalBounds().reduced(10);
    const auto padding = 10;
    const auto sliderWidth = bounds.getWidth() / 4 - padding;
    const auto sliderHeight = bounds.getHeight();
    const auto sliderStartX = 0;
    const auto sliderStartY = 0;

    attackSlider.setBounds(sliderStartX, sliderStartY, sliderWidth, sliderHeight);
    decaySlider.setBounds(attackSlider.getRight() + padding, sliderStartY, sliderWidth, sliderHeight);
    sustainSlider.setBounds(decaySlider.getRight() + padding, sliderStartY, sliderWidth, sliderHeight);
    releaseSlider.setBounds(sustainSlider.getRight() + padding, sliderStartY, sliderWidth, sliderHeight);

}

void AdsrComponent::setSliderParams(juce::AudioProcessorValueTreeState& treeState ,
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>& attachment,
    juce::Slider& sldr, juce::String paramId)
{
    sldr.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
    sldr.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 50, 25);
    addAndMakeVisible(sldr);

    attachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(treeState, paramId, sldr);
}
