#pragma once

#include <map>
#include <vector>

namespace dsp {

class MidiMessage {

public:
    MidiMessage(uint8_t byte0);
    MidiMessage(uint8_t byte0, uint8_t byte1);
    MidiMessage(uint8_t byte0, uint8_t byte1, uint8_t byte2);
    MidiMessage(std::vector<uint8_t> bytes);

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

private:
    std::vector<uint8_t> bytes;
};

class TimedMidiMessage {

public:
    int samplePosition;

    TimedMidiMessage(MidiMessage midiMessage, int samplePosition = 0);

    MidiMessage getMessage() const;

private:
    MidiMessage midiMessage;
};

class MidiData {

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

    MidiData();

    Iterator begin() const;
    Iterator end() const;

    void addEvent(const MidiMessage &midiMessage, int sample);
    void addEvents(const MidiData &midiData, int startSample, int numSamples, int sampleDeltaToAdd);
    void clear();

private:
    std::multimap<int, TimedMidiMessage> events;
};

} // namespace dsp
