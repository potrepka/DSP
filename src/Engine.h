#pragma once

#include "MidiProcessor.h"
#include "NodeProcessor.h"

#if !defined(DSP_NO_RTAUDIO) && !defined(DSP_USE_JUCE)
#define DSP_USE_RTAUDIO
#endif

#ifdef DSP_USE_RTAUDIO
#include "RtAudio.h"
#endif

namespace dsp {

class Engine : public Lockable {

public:
    Engine();
    ~Engine();

    std::vector<unsigned int> getInputDevices();
    std::vector<unsigned int> getOutputDevices();
    std::vector<unsigned int> getSampleRates(unsigned int inputDevice, unsigned int outputDevice);

    unsigned int getDefaultInputDevice();
    unsigned int getDefaultOutputDevice();
    unsigned int getDefaultSampleRate(unsigned int inputDevice, unsigned int outputDevice);

    void setup(unsigned int inputDevice, unsigned int outputDevice, unsigned int numSamples, unsigned int sampleRate);
    void start();

    std::string getDeviceName(unsigned int device);
    std::string getInputDeviceName() const;
    std::string getOutputDeviceName() const;

    unsigned int getNumInputChannels() const;
    unsigned int getNumOutputChannels() const;
    unsigned int getNumSamples() const;
    unsigned int getSampleRate() const;

    AudioBuffer<Sample> &getAudioBuffer();
    void processAudioBufferNoLock(Sample *inputBuffer, Sample *outputBuffer);

    std::shared_ptr<NodeProcessor> getNodeProcessor() const;
    std::shared_ptr<MidiProcessor> getMidiProcessor() const;

private:
    AudioBuffer<Sample> audioBuffer;
    const std::shared_ptr<NodeProcessor> nodeProcessor;
    const std::shared_ptr<MidiProcessor> midiProcessor;
#ifdef DSP_USE_RTAUDIO
    RtAudio dac;
#endif
    std::string inputDeviceName;
    std::string outputDeviceName;
    unsigned int numInputChannels;
    unsigned int numOutputChannels;
    unsigned int numSamples;
    unsigned int sampleRate;
#ifdef DSP_USE_RTAUDIO
    static int tick(void *outputBuffer,
                    void *inputBuffer,
                    unsigned int nBufferFrames,
                    double streamTime,
                    RtAudioStreamStatus status,
                    void *pointer);
#endif
    static void process(Sample *inputBuffer, Sample *outputBuffer, Engine *engine);
    unsigned int getDeviceCount();
    std::vector<unsigned int> getInputSampleRates(unsigned int inputDevice);
    std::vector<unsigned int> getOutputSampleRates(unsigned int outputDevice);
};

} // namespace dsp
