/*
  ==============================================================================

    MidiLogger.cpp
    Created: 9 Oct 2022 8:15:21pm
    Author:  Andreas Sandersen

  ==============================================================================
*/

#include "MidiLogger.h"

MidiLogger::MidiLogger()
{
    startTimerHz (60);

}

MidiLogger::~MidiLogger()
{
    
}

void MidiLogger::push (const MidiBuffer& buffer)
{
    for (const auto metadata : buffer)
        fifo.write (1).forEach ([&] (int dest) { messages[(size_t) dest] = metadata.getMessage(); });
}

template <typename OutputIt>
void MidiLogger::pop (OutputIt out)
{
    fifo.read (fifo.getNumReady()).forEach ([&] (int source) { *out++ = messages[(size_t) source]; });
}
