#include "EditorContent.h"
#include "MyColours.h"

EditorContent::EditorContent(BeeVerbAudioProcessor& p, juce::UndoManager& um)
	: apvts(p.getPluginState()), undoManager(um),
	sizeDial(*apvts.getParameter("SIZE"), &um),
	dampDial(*apvts.getParameter("DAMP"), &um),
	widthDial(*apvts.getParameter("WIDTH"), &um),
	mixDial(*apvts.getParameter("MIX"), &um),
	freezeAttachment(*apvts.getParameter("FREEZE"), freezeButton, &um)
{
	setWantsKeyboardFocus(true);
	addAndMakeVisible(sizeDial);
	addAndMakeVisible(dampDial);
	addAndMakeVisible(widthDial);
	addAndMakeVisible(mixDial);
	addAndMakeVisible(freezeButton);
}

void EditorContent::resized()
{
	const juce::Rectangle<int> baseDialBounds { 0, 73, 80, 96 };
	sizeDial.setBounds(baseDialBounds.withX(46));
	dampDial.setBounds(baseDialBounds.withX(144));
	widthDial.setBounds(baseDialBounds.withX(342));
	mixDial.setBounds(baseDialBounds.withX(440));
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

	const auto cmdShiftZ = juce::KeyPress('z', juce::ModifierKeys::commandModifier | juce::ModifierKeys::shiftModifier, 0);

	if (k == cmdShiftZ && undoManager.canRedo())
	{
		undoManager.redo();
		return true;
	}

	return false;
}