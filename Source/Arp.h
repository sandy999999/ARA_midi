/*
  ==============================================================================

    Arp.h
    Created: 9 Oct 2022 7:52:26pm
    Author:  Andreas Sandersen

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include <iterator>

class Arp  : public juce::Component
{
public:
    Arp();
    ~Arp() override;

private:

    ValueTree state { "state" };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Arp)
};
