#pragma once

#include "Node.h"

namespace dsp {

class NodeProcessor : public Lockable {

public:
    NodeProcessor(size_t numInputChannels, size_t numOutputChannels, size_t numSamples, double sampleRate);
    ~NodeProcessor();

    bool isActive() const;
    void setActive(bool active);

    std::shared_ptr<Output> getAudioInput() const;
    std::shared_ptr<Input> getAudioOutput() const;

    std::shared_ptr<Output> getAudioInputClipping() const;
    std::shared_ptr<Output> getAudioOutputClipping() const;

    size_t getNumInputChannels() const;
    void setNumInputChannels(size_t numChannels);

    size_t getNumOutputChannels() const;
    void setNumOutputChannels(size_t numChannels);

    size_t getNumSamples() const;
    void setNumSamples(size_t numSamples);

    double getSampleRate() const;
    void setSampleRate(double sampleRate);

    void setInputSize(size_t numChannels, size_t numSamples);
    void setOutputSize(size_t numChannels, size_t numSamples);

    std::shared_ptr<Node> getDefaultNode() const;

    std::vector<std::shared_ptr<Node>> &getNodes();
    std::shared_ptr<MidiBuffer> getInputMessages() const;
    std::shared_ptr<MidiBuffer> getOutputMessages() const;

    template <typename T>
    void process(AudioBuffer<T> &audioBuffer, MidiBuffer &midiBuffer);

private:
    const std::shared_ptr<Output> audioInput;
    const std::shared_ptr<Input> audioOutput;
    const std::shared_ptr<Output> audioInputClipping;
    const std::shared_ptr<Output> audioOutputClipping;
    bool active;
    size_t numInputChannels;
    size_t numOutputChannels;
    size_t numSamples;
    double sampleRate;
    std::shared_ptr<Node> defaultNode;

    std::vector<std::shared_ptr<Node>> nodes;
    const std::shared_ptr<MidiBuffer> inputMessages;
    const std::shared_ptr<MidiBuffer> outputMessages;

    void processClipping(Wrapper &audioWrapper, Wrapper &audioClippingWrapper);
};

} // namespace dsp
