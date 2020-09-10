#pragma once

#include "MidiMessage.h"

#include <map>

namespace dsp {

class MidiBuffer {

public:
    class Iterator {

    public:
        Iterator(std::multimap<size_t, TimedMidiMessage>::const_iterator it);
        Iterator &operator++();
        bool operator==(const Iterator &other) const;
        bool operator!=(const Iterator &other) const;
        TimedMidiMessage operator*() const;

    private:
        std::map<size_t, TimedMidiMessage>::const_iterator it;
    };

    MidiBuffer();

    Iterator begin() const;
    Iterator end() const;

    void addEvent(const MidiMessage &midiMessage, size_t sample);
    void addEvents(const MidiBuffer &midiBuffer, size_t startSample, size_t numSamples, size_t sampleDeltaToAdd);
    void clear();

private:
    std::multimap<size_t, TimedMidiMessage> events;
};

} // namespace dsp
