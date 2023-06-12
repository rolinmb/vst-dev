#pragma once

#include <JuceHeader.h>
#include "../../Source/PluginProcessor.h"
#include "Dial.h"
#include "FreezeButton.h"

class EditorContent : public juce::Component 
{
public:
	EditorContent(BeeVerbAudioProcessor& p, juce::UndoManager& um);

	void resized() override;

	bool keyPressed(const juce::KeyPress& k) override;

private:
	juce::AudioProcessorValueTreeState& apvts;
	juce::UndoManager& undoManager;

	Dial sizeDial;
	Dial dampDial;
	Dial widthDial;
	Dial mixDial;

	FreezeButton freezeButton;

	juce::ButtonParameterAttachment freezeAttachment;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(EditorContent)
};