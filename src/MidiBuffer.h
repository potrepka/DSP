#pragma once

#include "MidiMessage.h"

#include <map>

namespace dsp {

class MidiBuffer {

public:
    class Iterator {

    public:
        Iterator(std::multimap<int, TimedMidiMessage>::const_iterator it);
        Iterator &operator++();
        bool operator==(const Iterator &other) const;
        bool operator!=(const Iterator &other) const;
        TimedMidiMessage operator*() const;

    private:
        std::map<int, TimedMidiMessage>::const_iterator it;
    };

    MidiBuffer();

    Iterator begin() const;
    Iterator end() const;

    void addEvent(const MidiMessage &midiMessage, int sample);
    void addEvents(const MidiBuffer &midiBuffer, int startSample, int numSamples, int sampleDeltaToAdd);
    void clear();

private:
    std::multimap<int, TimedMidiMessage> events;
};

} // namespace dsp
