#pragma once

#include "Lockable.h"
#include "MidiBuffer.h"
#include <string>
#include <thread>

#if !defined(DSP_NO_RTMIDI) && !defined(DSP_USE_JUCE)
#define DSP_USE_RTMIDI
#endif

#ifdef DSP_USE_RTMIDI
#include "RtMidi.h"
#endif

namespace dsp {

class MidiProcessor : public Lockable {

public:
    class Input : public Lockable {

    public:
        static void callback(double delta, std::vector<unsigned char> *message, void *data);

        Input(unsigned int port);

        std::string getDeviceName() const;
        void setPort(unsigned int port);

        std::multimap<double, MidiMessage> &getMessages();

    private:
#ifdef DSP_USE_RTMIDI
        RtMidiIn midiIn;
#endif
        std::string deviceName;
        double messageTime;
        std::multimap<double, MidiMessage> messages;
    };

    class Output : public Lockable {

    public:
        Output(unsigned int port);

        std::string getDeviceName() const;
        void setPort(unsigned int port);

        void sendMessageWithDelay(std::vector<unsigned char> bytes, int64_t nanoseconds);

    private:
#ifdef DSP_USE_RTMIDI
        RtMidiOut midiOut;
#endif
        std::string deviceName;
    };

    static unsigned int getNumMidiInputPorts();
    static unsigned int getNumMidiOutputPorts();
    static std::string getMidiInputName(unsigned int port);
    static std::string getMidiOutputName(unsigned int port);

    MidiProcessor();

    size_t getNumSamples() const;
    void setNumSamples(size_t numSamples);

    double getSampleRate() const;
    void setSampleRate(double sampleRate);

    MidiBuffer &getMidiBuffer();
    std::vector<std::shared_ptr<Input>> &getInputs();
    std::vector<std::shared_ptr<Output>> &getOutputs();

    void processInputs();
    void processOutputs();

private:
#ifdef DSP_USE_RTMIDI
    static RtMidiIn midiIn;
    static RtMidiOut midiOut;
#endif
    size_t numSamples;
    double sampleRate;
    double oneOverSampleRate;
    double messageTime;
    MidiBuffer midiBuffer;
    std::vector<std::shared_ptr<Input>> inputs;
    std::vector<std::shared_ptr<Output>> outputs;
};

} // namespace dsp
