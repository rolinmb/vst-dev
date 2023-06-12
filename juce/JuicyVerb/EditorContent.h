#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "GUI/FreezeButton.h"

class EditorContent  : public juce::Component
{
public:
    EditorContent(JuicyVerbAudioProcessor& p, juce::UndoManager& um);
   
    void resized() override;

    bool keyPressed(const juce::KeyPress& k) override;

private:
    juce::AudioProcessorValueTreeState& apvts;
    juce::UndoManager& undoManager;

    juce::Slider sizeSlider;
    juce::Slider dampSlider;
    juce::Slider widthSlider;
    juce::Slider mixSlider;

    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> sizeAttach;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> dampAttach;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> widthAttach;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> mixAttach;

    FreezeButton freezeButton;

    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> freezeAttach;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (EditorContent)
};
