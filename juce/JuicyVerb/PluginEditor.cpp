#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
JuicyVerbAudioProcessorEditor::JuicyVerbAudioProcessorEditor (JuicyVerbAudioProcessor& p, juce::UndoManager& um)
    : AudioProcessorEditor (&p), editorContent(p, um)
{
    const auto ratio = static_cast<double> (defaultWidth) / defaultHeight;
    setResizable(false, true);
    getConstrainer()->setFixedAspectRatio(ratio);
    getConstrainer()->setSizeLimits(defaultWidth, defaultHeight, defaultWidth * 2, defaultHeight * 2);
    setSize (defaultWidth, defaultHeight);
    editorContent.setSize(defaultWidth, defaultHeight);

    addAndMakeVisible(editorContent);
}

JuicyVerbAudioProcessorEditor::~JuicyVerbAudioProcessorEditor()
{
}

//==============================================================================
void JuicyVerbAudioProcessorEditor::paint (juce::Graphics& g)
{
    g.fillAll(juce::Colours::black);
}

void JuicyVerbAudioProcessorEditor::resized()
{
    const auto factor = static_cast<float> (getWidth()) / defaultWidth;
    editorContent.setTransform(juce::AffineTransform::scale(factor));
}
