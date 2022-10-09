/*
  ==============================================================================

    Dial.h
    Created: 1 Apr 2022 1:54:30pm
    Author:  Andreas Sandersen

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "MyColors.h"
#include "MyLookAndFeel.h"

class Dial  : public juce::Slider
{
public:
    Dial();
    ~Dial() override;

    void paint (juce::Graphics& g) override;

    void mouseDown (const juce::MouseEvent& event) override;
    void mouseDrag (const juce::MouseEvent& event) override;
    void mouseUp (const juce::MouseEvent& event) override;

    bool keyPressed (const juce::KeyPress& key) override;

private:
    void drawFocusMark (juce::Graphics& g, juce::Colour colour);

    MyLookAndFeel lnf;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Dial)
};
