#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "EditorContent.h"

class JuicyVerbAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    JuicyVerbAudioProcessorEditor (JuicyVerbAudioProcessor&, juce::UndoManager& um);
    ~JuicyVerbAudioProcessorEditor() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    EditorContent editorContent;

    const int defaultWidth { 620 };
    const int defaultHeight { 500 };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (JuicyVerbAudioProcessorEditor)
};
