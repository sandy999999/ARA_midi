/*
  ==============================================================================

    MidiTable.h
    Created: 10 Oct 2022 12:00:46pm
    Author:  Andreas Sandersen

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include <iterator>


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
