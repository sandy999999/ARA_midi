/*
  ==============================================================================

    MidiLogger.h
    Created: 9 Oct 2022 8:15:21pm
    Author:  Andreas Sandersen

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include <iterator>

class MidiLogger  : public juce::Component,
                    public Timer
{
public:
    MidiLogger();
    ~MidiLogger() override;

    void push (const MidiBuffer& buffer);

    template <typename OutputIt>
    void pop (OutputIt out);

private:

    ValueTree state { "state" };
    MidiQueue queue;
    MidiListModel model; // The data to show in the UI. We keep it around in the processor so that
                         // the view is persistent even when the plugin UI is closed and reopened.
    
    static constexpr auto queueSize = 1 << 14;
    AbstractFifo fifo { queueSize };
    std::vector<MidiMessage> messages = std::vector<MidiMessage> (queueSize);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MidiLogger)
};
