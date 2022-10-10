/*
  ==============================================================================

    MidiList.h
    Created: 10 Oct 2022 12:03:06pm
    Author:  Andreas Sandersen

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include <iterator>

class MidiListModel
{
public:
    template <typename It>
    void addMessages (It begin, It end)
    {
        if (begin == end)
            return;

        const auto numNewMessages = (int) std::distance (begin, end);
        const auto numToAdd = juce::jmin (numToStore, numNewMessages);
        const auto numToRemove = jmax (0, (int) messages.size() + numToAdd - numToStore);
        messages.erase (messages.begin(), std::next (messages.begin(), numToRemove));
        messages.insert (messages.end(), std::prev (end, numToAdd), end);

        if (onChange != nullptr)
            onChange();
    }

    void clear()
    {
        messages.clear();

        if (onChange != nullptr)
            onChange();
    }

    const MidiMessage& operator[] (size_t ind) const     { return messages[ind]; }

    size_t size() const                                  { return messages.size(); }

    std::function<void()> onChange;

private:
    static constexpr auto numToStore = 1000;
    std::vector<MidiMessage> messages;
};

