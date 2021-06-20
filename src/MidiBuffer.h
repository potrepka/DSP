#pragma once

#ifdef DSP_USE_JUCE
#include "Constants.h"
#else
#include "MidiMessage.h"
#endif

#include <map>

namespace dsp {

#ifdef DSP_USE_JUCE
typedef juce::MidiMessage MidiMessage;
typedef juce::MidiBuffer MidiBuffer;
#else
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
#endif

} // namespace dsp
