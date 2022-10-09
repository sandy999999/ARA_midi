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

class MidiTable  : public Component,
                   private TableListBoxModel
{
public:
    MidiTable (MidiListModel& m)
        : messages (m)
    {
        addAndMakeVisible (table);

        table.setModel (this);
        table.setClickingTogglesRowSelection (false);
        table.setHeader ([&]
        {
            auto header = std::make_unique<TableHeaderComponent>();
            header->addColumn ("Message", messageColumn, 200, 30, -1, TableHeaderComponent::notSortable);
            header->addColumn ("Time",    timeColumn,    100, 30, -1, TableHeaderComponent::notSortable);
            header->addColumn ("Channel", channelColumn, 100, 30, -1, TableHeaderComponent::notSortable);
            header->addColumn ("Data",    dataColumn,    200, 30, -1, TableHeaderComponent::notSortable);
            return header;
        }());

        messages.onChange = [&] { table.updateContent(); };
    }

    ~MidiTable() override { messages.onChange = nullptr; }

    void resized() override { table.setBounds (getLocalBounds()); }

private:
    enum
    {
        messageColumn = 1,
        timeColumn,
        channelColumn,
        dataColumn
    };

    int getNumRows() override          { return (int) messages.size(); }

    void paintRowBackground (Graphics&, int, int, int, bool) override {}
    void paintCell (Graphics&, int, int, int, int, bool)     override {}

    Component* refreshComponentForCell (int rowNumber,
                                        int columnId,
                                        bool,
                                        Component* existingComponentToUpdate) override
    {
        delete existingComponentToUpdate;

        const auto index = (int) messages.size() - 1 - rowNumber;
        const auto message = messages[(size_t) index];

        return new Label ({}, [&]
        {
            switch (columnId)
            {
                case messageColumn: return getEventString (message);
                case timeColumn:    return String (message.getTimeStamp());
                case channelColumn: return String (message.getChannel());
                case dataColumn:    return getDataString (message);
                default:            break;
            }

            jassertfalse;
            return String();
        }());
    }

    static String getEventString (const MidiMessage& m)
    {
        if (m.isNoteOn())           return "Note on";
        if (m.isNoteOff())          return "Note off";
        if (m.isProgramChange())    return "Program change";
        if (m.isPitchWheel())       return "Pitch wheel";
        if (m.isAftertouch())       return "Aftertouch";
        if (m.isChannelPressure())  return "Channel pressure";
        if (m.isAllNotesOff())      return "All notes off";
        if (m.isAllSoundOff())      return "All sound off";
        if (m.isMetaEvent())        return "Meta event";

        if (m.isController())
        {
            const auto* name = MidiMessage::getControllerName (m.getControllerNumber());
            return "Controller " + (name == nullptr ? String (m.getControllerNumber()) : String (name));
        }

        return String::toHexString (m.getRawData(), m.getRawDataSize());
    }

    static String getDataString (const MidiMessage& m)
    {
        if (m.isNoteOn())           return MidiMessage::getMidiNoteName (m.getNoteNumber(), true, true, 3) + " Velocity " + String (m.getVelocity());
        if (m.isNoteOff())          return MidiMessage::getMidiNoteName (m.getNoteNumber(), true, true, 3) + " Velocity " + String (m.getVelocity());
        if (m.isProgramChange())    return String (m.getProgramChangeNumber());
        if (m.isPitchWheel())       return String (m.getPitchWheelValue());
        if (m.isAftertouch())       return MidiMessage::getMidiNoteName (m.getNoteNumber(), true, true, 3) +  ": " + String (m.getAfterTouchValue());
        if (m.isChannelPressure())  return String (m.getChannelPressureValue());
        if (m.isController())       return String (m.getControllerValue());

        return {};
    }

    MidiListModel& messages;
    TableListBox table;
};

