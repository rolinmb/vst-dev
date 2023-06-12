#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
BiccrushAudioProcessorEditor::BiccrushAudioProcessorEditor (BiccrushAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    noise.setName("Noise");
    noise.setSliderStyle(juce::Slider::LinearVertical);
    noise.setColour(juce::Slider::textBoxTextColourId, juce::Colours::white);
    noise.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 200, 20);
    noise.setRange(0.0f, 100.0f, 0.1f);
    noise.setValue(0.0f);
    addAndMakeVisible(noise);

    noiseAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(p.treeState, "NOISE", noise);

    bitsRedux.setName("Bits");
    bitsRedux.setRange(2.0f, 32.0f, 1.0f);
    bitsRedux.setValue(32.0f);
    bitsRedux.setSliderStyle(juce::Slider::LinearVertical);
    bitsRedux.setColour(juce::Slider::textBoxTextColourId, juce::Colours::white);
    bitsRedux.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 200, 20);
    addAndMakeVisible(bitsRedux);

    bitsAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(p.treeState, "BITS", bitsRedux);

    rateRedux.setName("Rate");
    rateRedux.setRange(1.0f, 50.0f, 1.0f);
    rateRedux.setValue(1.0f);
    rateRedux.setSliderStyle(juce::Slider::LinearVertical);
    rateRedux.setColour(juce::Slider::textBoxTextColourId, juce::Colours::white);
    rateRedux.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 200, 20);
    addAndMakeVisible(rateRedux);

    rateAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(p.treeState, "RATE", rateRedux);

    setSize (400, 300);
}

BiccrushAudioProcessorEditor::~BiccrushAudioProcessorEditor()
{
}

void BiccrushAudioProcessorEditor::paint (juce::Graphics& g)
{
    g.fillAll(juce::Colours::midnightblue.withMultipliedBrightness(.4));
}

void BiccrushAudioProcessorEditor::resized()
{
    int margin = 10;
    int w = 60;
    int y = 50;
    noise.setBounds(getWidth()/4 - w/2, y, w, getHeight() - y - margin);
    bitsRedux.setBounds(2*getWidth()/4 - w/2, y, w, getHeight() - y - margin);
    rateRedux.setBounds(3*getWidth()/4 - w/2, y, w, getHeight() - y - margin);

}
