#pragma once

#include "Node.h"

namespace dsp {

class NodeProcessor : public Lockable {

public:
    NodeProcessor(int numInputChannels, int numOutputChannels, int numSamples, double sampleRate);
    ~NodeProcessor();

    std::shared_ptr<Output> getAudioInput() const;
    std::shared_ptr<Input> getAudioOutput() const;

    std::shared_ptr<Output> getAudioInputClipping() const;
    std::shared_ptr<Output> getAudioOutputClipping() const;

    int getNumInputChannels() const;
    void setNumInputChannels(int numChannels);

    int getNumOutputChannels() const;
    void setNumOutputChannels(int numChannels);

    int getNumSamples() const;
    void setNumSamples(int numSamples);

    double getSampleRate() const;
    void setSampleRate(double sampleRate);

    void setInputSize(int numChannels, int numSamples);
    void setOutputSize(int numChannels, int numSamples);

    std::vector<std::shared_ptr<Node>> &getNodes();
    std::shared_ptr<MidiBuffer> getInputMessages() const;
    std::shared_ptr<MidiBuffer> getOutputMessages() const;

    template <typename T>
    void process(AudioBuffer<T> &audioBuffer, MidiBuffer &midiBuffer);

private:
    std::shared_ptr<Output> audioInput;
    std::shared_ptr<Input> audioOutput;
    std::shared_ptr<Output> audioInputClipping;
    std::shared_ptr<Output> audioOutputClipping;
    int numInputChannels;
    int numOutputChannels;
    int numSamples;
    double sampleRate;

    std::vector<std::shared_ptr<Node>> nodes;
    std::shared_ptr<MidiBuffer> inputMessages;
    std::shared_ptr<MidiBuffer> outputMessages;

    void processClipping(Wrapper &audioWrapper, Wrapper &audioClippingWrapper);
};

} // namespace dsp
