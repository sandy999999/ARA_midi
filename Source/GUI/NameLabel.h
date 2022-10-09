/*
  ==============================================================================

    NameLabel.h
    Created: 1 Apr 2022 1:55:03pm
    Author:  Andreas Sandersen

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class NameLabel  : public juce::Label
{
public:
    NameLabel();

    void resized() override;
};
