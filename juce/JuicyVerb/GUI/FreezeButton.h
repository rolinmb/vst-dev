#pragma once

#include <JuceHeader.h>


class FreezeButton : public juce::Button 
{
public:
    FreezeButton();

    void paint(juce::Graphics& g) override;
    void resized() override;

    void mouseDown(const juce::MouseEvent& event) override;
    void mouseUp(const juce::MouseEvent& event) override;

    void paintButton(juce::Graphics& g,
                     bool shouldDrawButtonAsHighlighted,
                     bool shouldDrawButtonAsDown) override;

private:
    juce::Path freezeIconPath;
    juce::Rectangle<float> freezeIconBounds;
    juce::Colour freezeColour { juce::Colours::dimgrey };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(FreezeButton)

};
