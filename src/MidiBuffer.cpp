#include "MidiBuffer.h"

dsp::MidiBuffer::Iterator::Iterator(std::multimap<int, TimedMidiMessage>::const_iterator it)
        : it(it) {}

dsp::MidiBuffer::Iterator &dsp::MidiBuffer::Iterator::operator++() {
    ++it;
    return *this;
}

bool dsp::MidiBuffer::Iterator::operator==(const Iterator &other) const {
    return it == other.it;
}

bool dsp::MidiBuffer::Iterator::operator!=(const Iterator &other) const {
    return it != other.it;
}

dsp::TimedMidiMessage dsp::MidiBuffer::Iterator::operator*() const {
    return it->second;
}

dsp::MidiBuffer::MidiBuffer() {}

dsp::MidiBuffer::Iterator dsp::MidiBuffer::begin() const {
    return Iterator(events.begin());
}

dsp::MidiBuffer::Iterator dsp::MidiBuffer::end() const {
    return Iterator(events.end());
}

void dsp::MidiBuffer::addEvent(const MidiMessage &midiMessage, int sample) {
    events.insert({sample, TimedMidiMessage(midiMessage, sample)});
}

void dsp::MidiBuffer::addEvents(const MidiBuffer &midiData, int startSample, int numSamples, int sampleDeltaToAdd) {
    for (const auto meta : midiData) {
        if (meta.samplePosition >= startSample && meta.samplePosition < startSample + numSamples) {
            MidiMessage message = meta.getMessage();
            addEvent(message, meta.samplePosition + sampleDeltaToAdd);
        }
    }
}

void dsp::MidiBuffer::clear() {
    events.clear();
}
