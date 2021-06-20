#pragma once

#include "Buffer.h"

#include <queue>
#include <unordered_map>
#include <unordered_set>

namespace dsp {

class Node : public Lockable {

public:
    Node();
    virtual ~Node();

    bool isActive() const;
    void setActive(bool active);

    size_t getNumChannels() const;
    void setNumChannels(size_t numChannels);

    size_t getNumInputChannels() const;
    void setNumInputChannels(size_t numChannels);

    size_t getNumOutputChannels() const;
    void setNumOutputChannels(size_t numChannels);

    size_t getNumSamples() const;
    void setNumSamples(size_t numSamples);

    double getSampleRate() const;
    void setSampleRate(double sampleRate);

    double getOneOverNumSamples() const;
    double getOneOverSampleRate() const;

    std::vector<std::shared_ptr<Input>> &getInputs();
    std::vector<std::shared_ptr<Output>> &getOutputs();
    std::vector<std::shared_ptr<Node>> &getChildren();

    void addChild(std::shared_ptr<Node> node);
    void removeChild(std::shared_ptr<Node> node);
    void sortChildren();

    void disconnectAll();
    void process();

protected:
    std::vector<std::shared_ptr<Input>> inputs;
    std::vector<std::shared_ptr<Output>> outputs;
    std::vector<std::shared_ptr<Node>> children;

    virtual void setNumInputChannelsNoLock(size_t numInputChannels);
    virtual void setNumOutputChannelsNoLock(size_t numOutputChannels);
    virtual void setNumSamplesNoLock(size_t numSamples);
    virtual void setSampleRateNoLock(double sampleRate);
    virtual void processNoLock();

private:
    bool active;
    size_t numInputChannels;
    size_t numOutputChannels;
    size_t numSamples;
    double sampleRate;
    double oneOverNumSamples;
    double oneOverSampleRate;
};

} // namespace dsp
