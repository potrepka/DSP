#pragma once

#include "Connection.h"
#include "Runnable.h"
#include <queue>
#include <thread>

#define USE_RTMIDI 1

#if USE_RTMIDI
#include "RtMidi.h"
#endif

namespace dsp {

class Midi : public Runnable {

public:
    class TimedMessage {

    public:
        static const unsigned char MIDI_CHANNELS;
        static const unsigned char MIDI_NOTES;
        enum Type {
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
        struct compare {
            bool operator()(const TimedMessage &left, const TimedMessage &right) const {
                return left.time < right.time;
            }
        };
        TimedMessage(double time, std::vector<unsigned char> message);
        double getTime() const;
        std::vector<unsigned char> getMessage() const;
        Type getType() const;
        unsigned char getChannel() const;
        unsigned char getByte(std::size_t index) const;
        DSP_FLOAT getByteAsUnipolar(std::size_t index) const;
        void setByte(std::size_t index, unsigned char value);
        void setByteUsingUnipolar(std::size_t index, DSP_FLOAT unipolar);
        unsigned short getShort(std::size_t lsb, std::size_t msb) const;
        DSP_FLOAT getShortAsBipolar(std::size_t lsb, std::size_t msb) const;
        void setShort(std::size_t lsb, std::size_t msb, unsigned short value);
        void setShortUsingBipolar(std::size_t lsb, std::size_t msb, DSP_FLOAT bipolar);

    private:
        double time;
        std::vector<unsigned char> message;
    };

    class MidiInput : public Runnable {

    public:
        static void callback(double delta, std::vector<unsigned char> *message, void *data);
        MidiInput(unsigned int port);
        std::string getName() const;
        void setPort(unsigned int port);
        std::shared_ptr<Input> getNoteGate(unsigned char channel, unsigned char note);
        std::shared_ptr<Input> getNotePressure(unsigned char channel, unsigned char note);
        std::shared_ptr<Input> getControlChange(unsigned char channel, unsigned char control);
        std::shared_ptr<Input> getProgramChange(unsigned char channel);
        std::shared_ptr<Input> getChannelPressure(unsigned char channel);
        std::shared_ptr<Input> getPitchBend(unsigned char channel);
        void pushQueue(double delta, std::vector<unsigned char> &message);
        void runCallbacks(double delta, std::vector<unsigned char> &message);
        void addCallback(void (*callback)(double, std::vector<unsigned char>));
        void removeCallback(void (*callback)(double, std::vector<unsigned char>));
        void run();

    private:
#if USE_RTMIDI
        RtMidiIn midiIn;
#endif
        std::string name;
        double messageTime;
        unsigned long bufferSamples;
        std::priority_queue<TimedMessage, std::vector<TimedMessage>, TimedMessage::compare> queue;
        std::vector<void (*)(double, std::vector<unsigned char>)> callbacks;
        std::vector<std::vector<DSP_FLOAT>> noteGateState;
        std::vector<std::vector<DSP_FLOAT>> notePressureState;
        std::vector<std::vector<DSP_FLOAT>> controlChangeState;
        std::vector<DSP_FLOAT> programChangeState;
        std::vector<DSP_FLOAT> channelPressureState;
        std::vector<DSP_FLOAT> pitchBendState;
        DSP_FLOAT playingState;
        std::vector<std::vector<std::shared_ptr<Input>>> noteGate;
        std::vector<std::vector<std::shared_ptr<Input>>> notePressure;
        std::vector<std::vector<std::shared_ptr<Input>>> controlChange;
        std::vector<std::shared_ptr<Input>> programChange;
        std::vector<std::shared_ptr<Input>> channelPressure;
        std::vector<std::shared_ptr<Input>> pitchBend;
        std::shared_ptr<Input> playing;
        std::shared_ptr<Input> reset;
        std::shared_ptr<Input> clock;
    };

    class MidiOutput : public Runnable {

    public:
        MidiOutput(unsigned int port);
        std::string getName() const;
        void setPort(unsigned int port);
        std::shared_ptr<Output> getNoteGate(unsigned char channel, unsigned char note);
        std::shared_ptr<Output> getNotePressure(unsigned char channel, unsigned char note);
        std::shared_ptr<Output> getControlChange(unsigned char channel, unsigned char control);
        std::shared_ptr<Output> getProgramChange(unsigned char channel);
        std::shared_ptr<Output> getChannelPressure(unsigned char channel);
        std::shared_ptr<Output> getPitchBend(unsigned char channel);
        void sendMessageWithDelay(int64_t nanoseconds, std::vector<unsigned char> message);
        void run();

    private:
#if USE_RTMIDI
        RtMidiOut midiOut;
#endif
        std::string name;
        std::vector<std::vector<unsigned char>> noteGateState;
        std::vector<std::vector<unsigned char>> notePressureState;
        std::vector<std::vector<unsigned char>> controlChangeState;
        std::vector<unsigned char> programChangeState;
        std::vector<unsigned char> channelPressureState;
        std::vector<unsigned short> pitchBendState;
        bool playingState;
        std::vector<std::vector<std::shared_ptr<Output>>> noteGate;
        std::vector<std::vector<std::shared_ptr<Output>>> notePressure;
        std::vector<std::vector<std::shared_ptr<Output>>> controlChange;
        std::vector<std::shared_ptr<Output>> programChange;
        std::vector<std::shared_ptr<Output>> channelPressure;
        std::vector<std::shared_ptr<Output>> pitchBend;
        std::shared_ptr<Output> playing;
        std::shared_ptr<Output> reset;
        std::shared_ptr<Output> clock;
    };

    Midi();

    static unsigned int getNumMidiInputPorts();
    static unsigned int getNumMidiOutputPorts();
    static std::string getMidiInputName(unsigned int port);
    static std::string getMidiOutputName(unsigned int port);

    std::size_t getNumMidiInputs();
    std::size_t getNumMidiOutputs();
    std::shared_ptr<MidiInput> getMidiInput(std::size_t index);
    std::shared_ptr<MidiOutput> getMidiOutput(std::size_t index);
    void pushMidiInput(unsigned int port);
    void pushMidiOutput(unsigned int port);
    void removeMidiInput(std::size_t index);
    void removeMidiOutput(std::size_t index);

    void processInputs();
    void processOutputs();

private:
#if USE_RTMIDI
    static RtMidiIn midiIn;
    static RtMidiOut midiOut;
#endif
    std::vector<std::shared_ptr<MidiInput>> midiInputs;
    std::vector<std::shared_ptr<MidiOutput>> midiOutputs;
};

} // namespace dsp
