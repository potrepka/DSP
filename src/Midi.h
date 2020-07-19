#pragma once

#ifndef DSP_USE_RTMIDI
#define DSP_USE_RTMIDI 1
#endif

#include "Connection.h"
#include "Runnable.h"
#include <queue>
#include <thread>

#if DSP_USE_RTMIDI
#include "RtMidi.h"
#endif

namespace dsp {

class Midi : public Runnable {

    enum MessageType {
        MIDI_UNKNOWN = 0x00,
        MIDI_NOTE_OFF = 0x80,
        MIDI_NOTE_ON = 0x90,
        MIDI_POLY_AFTERTOUCH = 0xA0,
        MIDI_CONTROL_CHANGE = 0xB0,
        MIDI_PROGRAM_CHANGE = 0xC0,
        MIDI_AFTERTOUCH = 0xD0,
        MIDI_PITCH_BEND = 0xE0,
        MIDI_SYSEX = 0xF0,
        MIDI_TIME_CODE = 0xF1,
        MIDI_SONG_POS_POINTER = 0xF2,
        MIDI_SONG_SELECT = 0xF3,
        MIDI_TUNE_REQUEST = 0xF6,
        MIDI_SYSEX_END = 0xF7,
        MIDI_TIME_CLOCK = 0xF8,
        MIDI_START = 0xFA,
        MIDI_CONTINUE = 0xFB,
        MIDI_STOP = 0xFC,
        MIDI_ACTIVE_SENSING = 0xFE,
        MIDI_SYSTEM_RESET = 0xFF
    };

public:
    class TimedMessage {

    public:
        static const unsigned char MIDI_CHANNELS;
        static const unsigned char MIDI_NOTES;

        struct compare {
            bool operator()(const TimedMessage &left, const TimedMessage &right) const {
                return left.time < right.time;
            }
        };

        TimedMessage(double time, std::vector<unsigned char> bytes);

        double getTime() const;
        MessageType getType() const;
        std::string getTypeName() const;
        unsigned char getChannel() const;

        std::vector<unsigned char> getBytes() const;

        unsigned char getByte(unsigned int index) const;
        Sample getByteAsUnipolar(unsigned int index) const;
        void setByte(unsigned int index, unsigned char value);
        void setByteUsingUnipolar(unsigned int index, Sample unipolar);

        unsigned short getShort(unsigned int lsb, unsigned int msb) const;
        Sample getShortAsBipolar(unsigned int lsb, unsigned int msb) const;
        void setShort(unsigned int lsb, unsigned int msb, unsigned short value);
        void setShortUsingBipolar(unsigned int lsb, unsigned int msb, Sample bipolar);

    private:
        double time;
        std::vector<unsigned char> bytes;
    };

    class MidiInput : public Runnable {

    public:
        static void callback(double delta, std::vector<unsigned char> *message, void *data);

        MidiInput(unsigned int port);

        std::string getDeviceName() const;
        void setPort(unsigned int port);
        std::shared_ptr<Input> getNoteOnTrigger(unsigned char channel, unsigned char note) const;
        std::shared_ptr<Input> getNoteOffTrigger(unsigned char channel, unsigned char note) const;
        std::shared_ptr<Input> getNotePressure(unsigned char channel, unsigned char note) const;
        std::shared_ptr<Input> getControlChange(unsigned char channel, unsigned char control) const;
        std::shared_ptr<Input> getChannelPressure(unsigned char channel) const;
        std::shared_ptr<Input> getPitchBend(unsigned char channel) const;
        std::shared_ptr<Input> getPlayTrigger() const;
        std::shared_ptr<Input> getStopTrigger() const;
        std::shared_ptr<Input> getResetTrigger() const;
        std::shared_ptr<Input> getClockTrigger() const;

        unsigned int getNumCallbacks() const;
        std::shared_ptr<std::function<void(TimedMessage)>> getCallback(unsigned int index) const;
        void pushCallback(std::function<void(TimedMessage)> callback);
        void removeCallback(std::shared_ptr<std::function<void(TimedMessage)>> callback);

        void run();

    private:
        void pushQueue(double delta, std::vector<unsigned char> bytes);
        void runCallbacks(TimedMessage message);
#if DSP_USE_RTMIDI
        RtMidiIn midiIn;
#endif
        std::string deviceName;
        double messageTime;
        unsigned long bufferSamples;
        std::priority_queue<TimedMessage, std::vector<TimedMessage>, TimedMessage::compare> queue;
        std::vector<std::shared_ptr<std::function<void(TimedMessage)>>> callbacks;
        std::vector<Array> notePressureState;
        std::vector<Array> controlChangeState;
        Array channelPressureState;
        Array pitchBendState;
        std::vector<std::vector<std::shared_ptr<Input>>> noteOn;
        std::vector<std::vector<std::shared_ptr<Input>>> noteOff;
        std::vector<std::vector<std::shared_ptr<Input>>> notePressure;
        std::vector<std::vector<std::shared_ptr<Input>>> controlChange;
        std::vector<std::shared_ptr<Input>> channelPressure;
        std::vector<std::shared_ptr<Input>> pitchBend;
        std::shared_ptr<Input> play;
        std::shared_ptr<Input> stop;
        std::shared_ptr<Input> reset;
        std::shared_ptr<Input> clock;
    };

    class MidiOutput : public Runnable {

    public:
        MidiOutput(unsigned int port);

        std::string getDeviceName() const;
        void setPort(unsigned int port);
        std::shared_ptr<Output> getNoteOnTrigger(unsigned char channel, unsigned char note) const;
        std::shared_ptr<Output> getNoteOffTrigger(unsigned char channel, unsigned char note) const;
        std::shared_ptr<Output> getNotePressure(unsigned char channel, unsigned char note) const;
        std::shared_ptr<Output> getControlChange(unsigned char channel, unsigned char control) const;
        std::shared_ptr<Output> getChannelPressure(unsigned char channel) const;
        std::shared_ptr<Output> getPitchBend(unsigned char channel) const;
        std::shared_ptr<Output> getPlayTrigger() const;
        std::shared_ptr<Output> getStopTrigger() const;
        std::shared_ptr<Output> getResetTrigger() const;
        std::shared_ptr<Output> getClockTrigger() const;

        void run();

    private:
        void sendMessageWithDelay(int64_t nanoseconds, std::vector<unsigned char> bytes);
#if DSP_USE_RTMIDI
        RtMidiOut midiOut;
#endif
        std::string deviceName;
        std::vector<std::vector<unsigned char>> notePressureState;
        std::vector<std::vector<unsigned char>> controlChangeState;
        std::vector<unsigned char> channelPressureState;
        std::vector<unsigned short> pitchBendState;
        std::vector<std::vector<std::shared_ptr<Output>>> noteOn;
        std::vector<std::vector<std::shared_ptr<Output>>> noteOff;
        std::vector<std::vector<std::shared_ptr<Output>>> notePressure;
        std::vector<std::vector<std::shared_ptr<Output>>> controlChange;
        std::vector<std::shared_ptr<Output>> channelPressure;
        std::vector<std::shared_ptr<Output>> pitchBend;
        std::shared_ptr<Output> play;
        std::shared_ptr<Output> stop;
        std::shared_ptr<Output> reset;
        std::shared_ptr<Output> clock;
    };

    static std::string getMessageTypeName(MessageType type);

    static unsigned int getNumMidiInputPorts();
    static unsigned int getNumMidiOutputPorts();
    static std::string getMidiInputName(unsigned int port);
    static std::string getMidiOutputName(unsigned int port);

    unsigned int getNumMidiInputs() const;
    unsigned int getNumMidiOutputs() const;
    std::shared_ptr<MidiInput> getMidiInput(unsigned int index) const;
    std::shared_ptr<MidiOutput> getMidiOutput(unsigned int index) const;
    std::shared_ptr<MidiInput> pushMidiInput(unsigned int port);
    std::shared_ptr<MidiOutput> pushMidiOutput(unsigned int port);
    void removeMidiInput(std::shared_ptr<MidiInput> input);
    void removeMidiOutput(std::shared_ptr<MidiOutput> output);

    void processInputs();
    void processOutputs();

private:
#if DSP_USE_RTMIDI
    static RtMidiIn midiIn;
    static RtMidiOut midiOut;
#endif
    std::vector<std::shared_ptr<MidiInput>> midiInputs;
    std::vector<std::shared_ptr<MidiOutput>> midiOutputs;
};

} // namespace dsp
