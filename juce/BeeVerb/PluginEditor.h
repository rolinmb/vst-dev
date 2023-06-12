#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "../Builds/VisualStudio2022/EditorLnF.h"
#include "../Builds/VisualStudio2022/EditorContent.h"

//==============================================================================
class BeeVerbAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    BeeVerbAudioProcessorEditor (BeeVerbAudioProcessor&, juce::UndoManager& um);
    ~BeeVerbAudioProcessorEditor() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    EditorContent editorContent;
    
    const int defaultWidth { 560 };
    const int defaultHeight { 250};

    struct SharedLnF 
    {
        SharedLnF() { juce::LookAndFeel::setDefaultLookAndFeel(&editorLnF); };
        ~SharedLnF() { juce::LookAndFeel::setDefaultLookAndFeel(nullptr); };

        EditorLnF editorLnF;
    };

    juce::SharedResourcePointer<SharedLnF> lnf;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (BeeVerbAudioProcessorEditor)
};
