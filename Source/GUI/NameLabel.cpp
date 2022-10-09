/*
  ==============================================================================

    NameLabel.cpp
    Created: 1 Apr 2022 1:55:03pm
    Author:  Andreas Sandersen

  ==============================================================================
*/

#include "NameLabel.h"
#include "MyColors.h"

NameLabel::NameLabel()
{
    setColour (juce::Label::textColourId, MyColours::grey);
    setJustificationType (juce::Justification::centred);
}

void NameLabel::resized()
{
    if (getAttachedComponent() != nullptr)
        setFont (getAttachedComponent()->getHeight() * 0.28f);
}
