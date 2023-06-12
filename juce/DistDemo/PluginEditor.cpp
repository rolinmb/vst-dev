/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
DistDemoAudioProcessorEditor::DistDemoAudioProcessorEditor (DistDemoAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // Define mGainSlider style, range and default val
    mGainSlider.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
    mGainSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 50, 20);
    mGainSlider.setRange(-60.0f, 0.0f, 0.01f);
    mGainSlider.setValue(-20.0f);
    //mGainSlider.addListener(this);
    addAndMakeVisible(mGainSlider); // set component as visible

    gainAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(p.treeState, "GAIN", mGainSlider);

    setSize (200, 300);
}

DistDemoAudioProcessorEditor::~DistDemoAudioProcessorEditor()
{
}

//==============================================================================
void DistDemoAudioProcessorEditor::paint(juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll(juce::Colours::black);
}

void DistDemoAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    mGainSlider.setBounds(getWidth() / 2 - 50,getHeight() / 2 - 75,100,150);
}
/*
// juce::Slider::Listener implementation, assigning value from mGainSlider to mGain variable
void DistDemoAudioProcessorEditor::sliderValueChanged(juce::Slider* slider)
{
    if (slider == &mGainSlider)
    {
        audioProcessor.mGain = mGainSlider.getValue();
    }
}*/