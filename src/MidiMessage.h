#pragma once

#include "Constants.h"

#include <vector>

namespace dsp {

class MidiMessage {

public:
    MidiMessage(uint8_t byte0);
    MidiMessage(uint8_t byte0, uint8_t byte1);
    MidiMessage(uint8_t byte0, uint8_t byte1, uint8_t byte2);
    MidiMessage(const uint8_t *data, size_t size);

    bool isNote() const;
    bool isNoteOff() const;
    bool isNoteOn() const;
    bool isAftertouch() const;
    bool isController() const;
    bool isProgramChange() const;
    bool isChannelPressure() const;
    bool isPitchWheel() const;
    bool isAllNotesOff() const;
    bool isSongPositionPointer() const;
    bool isMidiClock() const;
    bool isMidiStart() const;
    bool isMidiContinue() const;
    bool isMidiStop() const;

    uint8_t getChannel() const;
    uint8_t getNoteNumber() const;
    uint8_t getVelocity() const;
    uint8_t getAfterTouchValue() const;
    uint8_t getControllerNumber() const;
    uint8_t getControllerValue() const;
    uint8_t getProgramChangeNumber() const;
    uint8_t getChannelPressureValue() const;
    int getPitchWheelValue() const;
    int getSongPositionPointerMidiBeat() const;

    static MidiMessage noteOff(uint8_t channel, uint8_t noteNumber, uint8_t velocity);
    static MidiMessage noteOn(uint8_t channel, uint8_t noteNumber, uint8_t velocity);
    static MidiMessage aftertouchChange(uint8_t channel, uint8_t noteNumber, uint8_t aftertouchValue);
    static MidiMessage controllerEvent(uint8_t channel, uint8_t controllerNumber, uint8_t controllerValue);
    static MidiMessage programChange(uint8_t channel, uint8_t programNumber);
    static MidiMessage channelPressureChange(uint8_t channel, uint8_t channelPressureValue);
    static MidiMessage pitchWheel(uint8_t channel, int pitchWheelValue);
    static MidiMessage allNotesOff(uint8_t channel);
    static MidiMessage songPositionPointer(int songPositionPointerMidiBeat);
    static MidiMessage midiClock();
    static MidiMessage midiStart();
    static MidiMessage midiContinue();
    static MidiMessage midiStop();

    std::vector<uint8_t> &getBytes();

    const uint8_t *getRawData() const;
    size_t getRawDataSize() const;

private:
    std::vector<uint8_t> bytes;
};

class TimedMidiMessage {

public:
    size_t samplePosition;

    TimedMidiMessage(MidiMessage midiMessage, size_t samplePosition = 0);

    MidiMessage getMessage() const;

private:
    MidiMessage midiMessage;
};

} // namespace dsp
