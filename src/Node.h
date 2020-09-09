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

    int getNumChannels() const;
    void setNumChannels(int numChannels);

    int getNumInputChannels() const;
    void setNumInputChannels(int numChannels);

    int getNumOutputChannels() const;
    void setNumOutputChannels(int numChannels);

    int getNumSamples() const;
    void setNumSamples(int numSamples);

    double getSampleRate() const;
    void setSampleRate(double sampleRate);

    double getOneOverNumSamples() const;
    double getOneOverSampleRate() const;

    std::vector<std::shared_ptr<Input>> &getInputs();
    std::vector<std::shared_ptr<Output>> &getOutputs();
    std::vector<std::shared_ptr<Node>> &getChildren();

    void sortChildren();

    void disconnectAll();
    void process();

protected:
    virtual void setNumInputChannelsNoLock(int numInputChannels);
    virtual void setNumOutputChannelsNoLock(int numOutputChannels);
    virtual void setNumSamplesNoLock(int numSamples);
    virtual void setSampleRateNoLock(double sampleRate);
    virtual void processNoLock() {}

private:
    bool active;
    int numInputChannels;
    int numOutputChannels;
    int numSamples;
    double sampleRate;
    double oneOverNumSamples;
    double oneOverSampleRate;
    std::vector<std::shared_ptr<Input>> inputs;
    std::vector<std::shared_ptr<Output>> outputs;
    std::vector<std::shared_ptr<Node>> children;
};

} // namespace dsp
