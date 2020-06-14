#pragma once

#include "Audio.h"

#define USE_RTAUDIO 1

#if USE_RTAUDIO
#include "RtAudio.h"
#endif

namespace dsp {

class Engine {

public:
    Engine();
    ~Engine();
    dsp::Audio *getAudio();
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

private:
    Audio *audio;
#if USE_RTAUDIO
    RtAudio dac;
#endif
    std::string inputDeviceName;
    std::string outputDeviceName;
    unsigned int numInputChannels;
    unsigned int numOutputChannels;
    unsigned int sampleRate;
    unsigned int bufferSize;
};

#if USE_RTAUDIO
int tick(void *outputBuffer,
         void *inputBuffer,
         unsigned int nBufferFrames,
         double streamTime,
         RtAudioStreamStatus status,
         void *pointer);
#endif

void process(double *inputBuffer,
             double *outputBuffer,
             unsigned int numFrames,
             unsigned int numInputChannels,
             unsigned int numOutputChannels,
             Engine *engine);

} // namespace dsp
