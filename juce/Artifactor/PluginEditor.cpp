#include "PluginProcessor.h"
#include "PluginEditor.h"

ArtifactorAudioProcessorEditor::ArtifactorAudioProcessorEditor (ArtifactorAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor(p)
{
    buildSliderWithAttachment(p.getPluginState(), mGainSlider, gainAttachment, mGainLabel,
        0.f, 1.5f, 0.01f, 1.f, "GAIN", "Gain");

    buildSliderWithAttachment(p.getPluginState(), mRangeSlider, rangeAttachment, mRangeLabel,
        0.f, 3000.f, 1.f, 1.f, "RANGE", "Range");

    buildSliderWithAttachment(p.getPluginState(), mBlendSlider, blendAttachment, mBlendLabel,
        0.f, 1.f, 0.01f, 1.f, "BLEND", "Blend");

    buildSliderWithAttachment(p.getPluginState(), mVolumeSlider, volumeAttachment, mVolumeLabel,
        0.f, 3.f, 0.01f, 1.f, "VOLUME", "Volume");

    juce::StringArray fuzzChoices {"Regular Fuzz", "Exponential Fuzz", "Arctan Fuzz"};
    fuzzSelector.addItemList(fuzzChoices, 1);
    addAndMakeVisible(fuzzSelector);

    selectAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(p.getPluginState(), "FUZZ TYPE", fuzzSelector);

    setSize(750, 750);
}

ArtifactorAudioProcessorEditor::~ArtifactorAudioProcessorEditor()
{
}

void ArtifactorAudioProcessorEditor::paint (juce::Graphics& g)
{
    g.fillAll(juce::Colours::black);
}

void ArtifactorAudioProcessorEditor::resized()
{
    mGainSlider.setBounds(getWidth() / 2.25 - 50, getHeight() / 2 - 75, 100, 150);
    mRangeSlider.setBounds(getWidth() / 2.25 - 125, getHeight() / 2 - 75, 100, 150);
    mBlendSlider.setBounds(getWidth() / 2.25 - 200, getHeight() / 2 - 75, 100, 150);
    mVolumeSlider.setBounds(getWidth() / 2.25 - 275, getHeight() / 2 - 75, 100, 150);
    mGainLabel.setBounds(getWidth() / 2.25 - 25, 212.5, 100, 50);
    mRangeLabel.setBounds(getWidth() / 2.25 - 100, 212.5, 100, 50);
    mBlendLabel.setBounds(getWidth() / 2.25 - 175, 212.5, 100, 50);
    mVolumeLabel.setBounds(getWidth() / 2.25 - 250, 212.5, 100, 50);
    fuzzSelector.setBounds(20, 20, 150, 42.5);
}

void ArtifactorAudioProcessorEditor::buildSliderWithAttachment(juce::AudioProcessorValueTreeState& state,
    juce::Slider& sldr, std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>& atchmnt, juce::Label& lbl,
    float minVal, float maxVal, float interVal, float defaultVal, juce::String name, juce::String labelText)
{
    sldr.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
    sldr.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 50, 20);
    sldr.setRange(minVal, maxVal, interVal);
    sldr.setValue(defaultVal);
    addAndMakeVisible(sldr);

    lbl.setText(labelText, juce::dontSendNotification);
    lbl.attachToComponent(&sldr, false);
    addAndMakeVisible(lbl);

    atchmnt = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(state, name, sldr);
}