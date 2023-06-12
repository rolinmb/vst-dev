#include <JuceHeader.h>
#include "EditorContent.h"

//==============================================================================
EditorContent::EditorContent(JuicyVerbAudioProcessor& p, juce::UndoManager& um)
    : apvts(p.getPluginState()), undoManager(um)
{
    setWantsKeyboardFocus(true);

    sizeSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    sizeSlider.setColour(juce::Slider::textBoxTextColourId, juce::Colours::white);
    sizeSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, 20);
    sizeSlider.setRange(0.0f, 1.0f, 0.0001f);
    sizeSlider.setValue(0.5f);
    addAndMakeVisible(sizeSlider);
    sizeAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(p.getPluginState(), "SIZE", sizeSlider);

    dampSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    dampSlider.setColour(juce::Slider::textBoxTextColourId, juce::Colours::white);
    dampSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, 20);
    dampSlider.setRange(0.0f, 1.0f, 0.0001f);
    dampSlider.setValue(0.5f);
    addAndMakeVisible(dampSlider);
    dampAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(p.getPluginState(), "DAMP", dampSlider);

    widthSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    widthSlider.setColour(juce::Slider::textBoxTextColourId, juce::Colours::white);
    widthSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, 20);
    widthSlider.setRange(0.0f, 1.0f, 0.0001f);
    widthSlider.setValue(0.5f);
    addAndMakeVisible(widthSlider);
    widthAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(p.getPluginState(), "WIDTH", widthSlider);

    mixSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    mixSlider.setColour(juce::Slider::textBoxTextColourId, juce::Colours::white);
    mixSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, 20);
    mixSlider.setRange(0.0f, 1.0f, 0.0001f);
    mixSlider.setValue(0.5f);
    addAndMakeVisible(mixSlider);
    mixAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(p.getPluginState(), "MIX", mixSlider);

    addAndMakeVisible(freezeButton);
    freezeAttach = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(p.getPluginState(), "FREEZE", freezeButton);
}

void EditorContent::resized()
{
    const juce::Rectangle<int> baseSliderBounds{ 0, 73, 80, 96 };
    sizeSlider.setBounds(baseSliderBounds.withX(46));
    dampSlider.setBounds(baseSliderBounds.withX(144));
    widthSlider.setBounds(baseSliderBounds.withX(342));
    mixSlider.setBounds(baseSliderBounds.withX(440));

    freezeButton.setBounds(249, 110, 68, 32);
}

bool EditorContent::keyPressed(const juce::KeyPress& k)
{
    const auto cmdZ = juce::KeyPress('z', juce::ModifierKeys::commandModifier, 0);

    if (k == cmdZ && undoManager.canUndo())
    {
        undoManager.undo();
        return true;
    }

    const auto cmdShiftZ = juce::KeyPress('z', juce::ModifierKeys::commandModifier
        | juce::ModifierKeys::shiftModifier, 0);

    if (k == cmdShiftZ && undoManager.canRedo())
    {
        undoManager.redo();
        return true;
    }

    return false;
}