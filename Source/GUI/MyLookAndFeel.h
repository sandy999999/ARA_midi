/*
  ==============================================================================

    MyLookAndFeel.h
    Created: 1 Apr 2022 1:54:55pm
    Author:  Andreas Sandersen

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "MyColors.h"
#include "ParamTextBox.h"

class MyLookAndFeel : public juce::LookAndFeel_V4
{
public:
    MyLookAndFeel();

    juce::Slider::SliderLayout getSliderLayout (juce::Slider& slider) override;

    void drawRotarySlider (juce::Graphics&, int x, int y, int width, int height,
                           float sliderPosProportional, float rotaryStartAngle,
                           float rotaryEndAngle, juce::Slider&) override;

    ParamTextBox* createSliderTextBox (juce::Slider& slider) override;
    
    juce::Font getTextButtonFont (juce::TextButton&, int buttonHeight) override;

    juce::CaretComponent* createCaretComponent (juce::Component* keyFocusOwner) override;

    void drawButtonBackground (juce::Graphics& g, juce::Button& button,
                               const juce::Colour& backgroundColour,
                               bool shouldDrawButtonAsHighlighted,
                               bool shouldDrawButtonAsDown) override;

    void drawCornerResizer (juce::Graphics& g, int w, int h, bool isMouseOver, bool isMouseDragging) override;
};
