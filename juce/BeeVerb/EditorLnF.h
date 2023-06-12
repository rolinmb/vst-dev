#pragma once

#include <JuceHeader.h>

class EditorLnF : public juce::LookAndFeel_V4
{
public:
	EditorLnF();

	void drawCornerResizer(juce::Graphics& g, int w, int h, bool isMouseOver, bool isMouseDragging) override;

private:

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(EditorLnF)
};