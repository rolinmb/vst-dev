#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
YoutubeDistortionAudioProcessorEditor::YoutubeDistortionAudioProcessorEditor (YoutubeDistortionAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    mDriveSlider.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
    mDriveSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 50, 20);
    mDriveSlider.setRange(0.0f, 1.0f, 0.01f);
    mDriveSlider.setValue(0.5f);
    addAndMakeVisible(mDriveSlider);

    mDriveLabel.setText("Drive", juce::dontSendNotification);
    mDriveLabel.attachToComponent(&mDriveSlider, false);
    addAndMakeVisible(mDriveLabel);

    driveAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(p.treeState, "DRIVE", mDriveSlider);

    mRangeSlider.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
    mRangeSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 50, 20);
    mRangeSlider.setRange(0.0f, 1.0f, 0.01f);
    mRangeSlider.setValue(0.5f);
    addAndMakeVisible(mRangeSlider);

    mRangeLabel.setText("Range", juce::dontSendNotification);
    mRangeLabel.attachToComponent(&mRangeSlider, false);
    addAndMakeVisible(mRangeLabel);

    rangeAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(p.treeState, "RANGE", mRangeSlider);

    mBlendSlider.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
    mBlendSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 50, 20);
    mBlendSlider.setRange(0.0f, 1.0f, 0.01f);
    mBlendSlider.setValue(0.5f);
    addAndMakeVisible(mBlendSlider);

    mBlendLabel.setText("Blend", juce::dontSendNotification);
    mBlendLabel.attachToComponent(&mBlendSlider, false);
    addAndMakeVisible(mBlendLabel);

    blendAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(p.treeState, "BLEND", mBlendSlider);

    mVolumeSlider.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
    mVolumeSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 50, 20);
    mVolumeSlider.setRange(0.0f, 1.0f, 0.01f);
    mVolumeSlider.setValue(1.0f);
    addAndMakeVisible(mVolumeSlider);

    mVolumeLabel.setText("Volume", juce::dontSendNotification);
    mVolumeLabel.attachToComponent(&mVolumeSlider, false);
    addAndMakeVisible(mVolumeLabel);

    volumeAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(p.treeState, "VOLUME", mVolumeSlider);

    setSize(750, 300);
}

YoutubeDistortionAudioProcessorEditor::~YoutubeDistortionAudioProcessorEditor()
{
}

//==============================================================================
void YoutubeDistortionAudioProcessorEditor::paint (juce::Graphics& g)
{
    g.fillAll(juce::Colours::black);
}

void YoutubeDistortionAudioProcessorEditor::resized()
{
    mDriveSlider.setBounds(getWidth() / 2 - 50, getHeight() / 2 - 75, 100, 150);
    mRangeSlider.setBounds(getWidth() / 2 - 125, getHeight() / 2 - 75, 100, 150);
    mBlendSlider.setBounds(getWidth() / 2 - 200, getHeight() / 2 - 75, 100, 150);
    mVolumeSlider.setBounds(getWidth() / 2 - 275, getHeight() / 2 - 75, 100, 150);
    mDriveLabel.setBounds(getWidth() / 2 - 25, 25, 100, 50);
    mRangeLabel.setBounds(getWidth() / 2 - 100, 25, 100, 50);
    mBlendLabel.setBounds(getWidth() / 2 - 175, 25, 100, 50);
    mVolumeLabel.setBounds(getWidth() / 2 - 250, 25, 100, 50);
}