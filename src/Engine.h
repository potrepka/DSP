#pragma once

#include "Audio.h"
#include "Midi.h"

#define USE_RTAUDIO 1

#if USE_RTAUDIO
#include "RtAudio.h"
#endif

namespace dsp {

class Engine {

public:
    Engine();
    ~Engine();

    std::vector<unsigned int> getInputDevices();
    std::vector<unsigned int> getOutputDevices();
    std::vector<unsigned int> getAvailableSampleRates(unsigned int inputDevice, unsigned int outputDevice);

    void setup(unsigned int inputDevice, unsigned int outputDevice, unsigned int sampleRate, unsigned int bufferSize);
    void start();
    void stop();

    std::string getDeviceName(unsigned int device);
    std::string getInputDeviceName();
    std::string getOutputDeviceName();

    unsigned int getNumInputChannels();
    unsigned int getNumOutputChannels();
    unsigned int getSampleRate();
    unsigned int getBufferSize();

    std::shared_ptr<Unit::OutputParameter> getAudioInput();
    std::shared_ptr<Unit::InputParameter> getAudioOutput();

    unsigned int getNumUnits();
    std::shared_ptr<Unit> getUnit(unsigned int index);
    void pushUnit(std::shared_ptr<Unit> unit);
    void insertUnit(unsigned int index, std::shared_ptr<Unit> unit);
    void removeUnit(std::shared_ptr<Unit> unit);
    void removeUnit(unsigned int index);
    void sortUnits();

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
    unsigned int sampleRate;
    unsigned int bufferSize;

#if USE_RTAUDIO
    static int tick(void *outputBuffer,
                    void *inputBuffer,
                    unsigned int nBufferFrames,
                    double streamTime,
                    RtAudioStreamStatus status,
                    void *pointer);
#endif

    static void process(double *inputBuffer,
                        double *outputBuffer,
                        unsigned int numFrames,
                        unsigned int numInputChannels,
                        unsigned int numOutputChannels,
                        Engine *engine);
};

} // namespace dsp
