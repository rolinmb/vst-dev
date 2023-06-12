#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
YTDist2AudioProcessorEditor::YTDist2AudioProcessorEditor (YTDist2AudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    driveSliderVal = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(p.treeState, "Drive", driveKnob);
    rangeSliderVal = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(p.treeState, "Range", rangeKnob);
    blendSliderVal = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(p.treeState, "Blend", blendKnob);
    volumeSliderVal = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(p.treeState, "Volume", volumeKnob);

    driveKnob.setSliderStyle(juce::Slider::SliderStyle::Rotary);
    driveKnob.setTextBoxStyle(juce::Slider::NoTextBox, true, 100, 100);
    driveKnob.addListener(this);
    addAndMakeVisible(&driveKnob);

    rangeKnob.setSliderStyle(juce::Slider::SliderStyle::Rotary);
    rangeKnob.setTextBoxStyle(juce::Slider::NoTextBox, true, 100, 100);
    rangeKnob.addListener(this);
    addAndMakeVisible(&rangeKnob);

    blendKnob.setSliderStyle(juce::Slider::SliderStyle::Rotary);
    blendKnob.setTextBoxStyle(juce::Slider::NoTextBox, true, 100, 100);
    blendKnob.addListener(this);
    addAndMakeVisible(&blendKnob);

    volumeKnob.setSliderStyle(juce::Slider::SliderStyle::Rotary);
    volumeKnob.setTextBoxStyle(juce::Slider::NoTextBox, true, 100, 100);
    volumeKnob.addListener(this);
    addAndMakeVisible(&volumeKnob);

    setSize(500, 200);
}

YTDist2AudioProcessorEditor::~YTDist2AudioProcessorEditor()
{
}

//==============================================================================
void YTDist2AudioProcessorEditor::paint (juce::Graphics& g)
{
    g.fillAll(juce::Colours::black);
}

void YTDist2AudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    driveKnob.setBounds(((getWidth() / 5) * 1) - 50, (getHeight() / 2) - 50, 100, 100);
    rangeKnob.setBounds(((getWidth() / 5) * 2) - 50, (getHeight() / 2) - 50, 100, 100);
    blendKnob.setBounds(((getWidth() / 5) * 3) - 50, (getHeight() / 2) - 50, 100, 100);
    volumeKnob.setBounds(((getWidth() / 5) * 4) - 50, (getHeight() / 2) - 50, 100, 100);
}

void YTDist2AudioProcessorEditor::sliderValueChanged(juce::Slider* slider)
{
    if (slider == &driveKnob)
    {
        audioProcessor.mDrive = driveKnob.getValue();
    }
    if (slider == &rangeKnob)
    {
        audioProcessor.mRange = rangeKnob.getValue();
    }
    if (slider == &blendKnob)
    {
        audioProcessor.mBlend = blendKnob.getValue();
    }
    if (slider == &volumeKnob)
    {
        audioProcessor.mVolume = volumeKnob.getValue();
    }
}