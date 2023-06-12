#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
JuceDelayAudioProcessorEditor::JuceDelayAudioProcessorEditor (JuceDelayAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    mDelaySlider.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
    mDelaySlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 50, 20);
    mDelaySlider.setRange(0.f, 1.f, 0.01f);
    mDelaySlider.setValue(0.f);
    addAndMakeVisible(mDelaySlider);

    mDelayLabel.setText("Delay", juce::dontSendNotification);
    mDelayLabel.attachToComponent(&mDelaySlider, false);
    addAndMakeVisible(mDelayLabel);

    delayAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(p.treeState, "DELAY", mDelaySlider);

    setSize (200, 300);
}

JuceDelayAudioProcessorEditor::~JuceDelayAudioProcessorEditor()
{
}

//==============================================================================
void JuceDelayAudioProcessorEditor::paint (juce::Graphics& g)
{
    g.fillAll(juce::Colours::black);
}

void JuceDelayAudioProcessorEditor::resized()
{
    mDelaySlider.setBounds(getWidth() / 2 - 50, getHeight() / 2 - 75, 100, 150);
}
