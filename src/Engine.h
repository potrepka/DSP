#pragma once

#include "Audio.h"
#include "Midi.h"
#include "Runnable.h"

#define USE_RTAUDIO 1

#if USE_RTAUDIO
#include "RtAudio.h"
#endif

namespace dsp {

class Engine : public Runnable {

public:
    Engine();
    ~Engine();

    void lockAudio();
    void unlockAudio();

    void lockMidi();
    void unlockMidi();

    std::vector<unsigned int> getInputDevices();
    std::vector<unsigned int> getOutputDevices();
    std::vector<unsigned int> getSampleRates(unsigned int inputDevice, unsigned int outputDevice);

    unsigned int getDefaultInputDevice();
    unsigned int getDefaultOutputDevice();
    unsigned int getDefaultSampleRate(unsigned int inputDevice, unsigned int outputDevice);

    void setup(unsigned int inputDevice, unsigned int outputDevice, unsigned int sampleRate, unsigned int bufferSize);
    void start();

    std::string getDeviceName(unsigned int device);
    std::string getInputDeviceName() const;
    std::string getOutputDeviceName() const;

    unsigned int getNumInputChannels() const;
    unsigned int getNumOutputChannels() const;

    std::shared_ptr<OutputParameter> getAudioInput() const;
    std::shared_ptr<InputParameter> getAudioOutput() const;
    std::shared_ptr<OutputParameter> getAudioInputClipping() const;
    std::shared_ptr<OutputParameter> getAudioOutputClipping() const;

    unsigned int getNumUnits() const;
    std::shared_ptr<Unit> getUnit(unsigned int index) const;
    void pushUnit(std::shared_ptr<Unit> unit, bool sort = false);
    void pushUnits(std::vector<std::shared_ptr<Unit>> units, bool sort = false);
    void replaceUnit(std::shared_ptr<Unit> unit, std::shared_ptr<Unit> replacement);
    void removeUnit(std::shared_ptr<Unit> unit);
    void removeUnits(std::vector<std::shared_ptr<Unit>> units);
    void sortUnits();

    unsigned int getNumMidiInputs() const;
    unsigned int getNumMidiOutputs() const;
    std::shared_ptr<Midi::MidiInput> getMidiInput(unsigned int index) const;
    std::shared_ptr<Midi::MidiOutput> getMidiOutput(unsigned int index) const;
    void pushMidiInput(unsigned int port);
    void pushMidiOutput(unsigned int port);
    void removeMidiInput(unsigned int index);
    void removeMidiOutput(unsigned int index);

private:
    std::shared_ptr<Audio> audio;
    std::shared_ptr<Midi> midi;
#if USE_RTAUDIO
    RtAudio dac;
#endif
    std::string inputDeviceName;
    std::string outputDeviceName;
    unsigned int numInputChannels;
    unsigned int numOutputChannels;
#if USE_RTAUDIO
    static int tick(void *outputBuffer,
                    void *inputBuffer,
                    unsigned int nBufferFrames,
                    double streamTime,
                    RtAudioStreamStatus status,
                    void *pointer);
#endif
    static void process(DSP_FLOAT *inputBuffer,
                        DSP_FLOAT *outputBuffer,
                        unsigned int numFrames,
                        unsigned int numInputChannels,
                        unsigned int numOutputChannels,
                        Engine *engine);
    unsigned int getDeviceCount();
    std::vector<unsigned int> getInputSampleRates(unsigned int inputDevice);
    std::vector<unsigned int> getOutputSampleRates(unsigned int outputDevice);
};

} // namespace dsp
