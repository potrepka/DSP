#include "Node.h"

dsp::Node::Node()
        : active(true)
        , numInputChannels(0)
        , numOutputChannels(0)
        , numSamples(0)
        , sampleRate(0.0)
        , oneOverNumSamples(0.0)
        , oneOverSampleRate(0.0) {}

dsp::Node::~Node() {
    disconnectAll();
}

bool dsp::Node::isActive() const {
    return active;
}

void dsp::Node::setActive(bool active) {
    lock();
    this->active = active;
    unlock();
}

int dsp::Node::getNumChannels() const {
    return numInputChannels < numOutputChannels ? numInputChannels : numOutputChannels;
}

void dsp::Node::setNumChannels(int numChannels) {
    DSP_ASSERT(numChannels >= 0);
    lock();
    setNumInputChannelsNoLock(numChannels);
    setNumOutputChannelsNoLock(numChannels);
    unlock();
}

int dsp::Node::getNumInputChannels() const {
    return numInputChannels;
}

void dsp::Node::setNumInputChannels(int numChannels) {
    DSP_ASSERT(numChannels >= 0);
    lock();
    setNumInputChannelsNoLock(numChannels);
    unlock();
}

int dsp::Node::getNumOutputChannels() const {
    return numOutputChannels;
}

void dsp::Node::setNumOutputChannels(int numChannels) {
    DSP_ASSERT(numChannels >= 0);
    lock();
    setNumOutputChannelsNoLock(numChannels);
    unlock();
}

int dsp::Node::getNumSamples() const {
    return numSamples;
}

void dsp::Node::setNumSamples(int numSamples) {
    DSP_ASSERT(numSamples >= 0);
    lock();
    setNumSamplesNoLock(numSamples);
    unlock();
}

double dsp::Node::getSampleRate() const {
    return sampleRate;
}

void dsp::Node::setSampleRate(double sampleRate) {
    DSP_ASSERT(sampleRate >= 0);
    lock();
    setSampleRateNoLock(sampleRate);
    unlock();
}

double dsp::Node::getOneOverSampleRate() const {
    return oneOverSampleRate;
}

double dsp::Node::getOneOverNumSamples() const {
    return oneOverNumSamples;
}

std::vector<std::shared_ptr<dsp::Input>> &dsp::Node::getInputs() {
    return inputs;
}

std::vector<std::shared_ptr<dsp::Output>> &dsp::Node::getOutputs() {
    return outputs;
}

std::vector<std::shared_ptr<dsp::Node>> &dsp::Node::getChildren() {
    return children;
}

void dsp::Node::sortChildren() {
    lock();
    if (children.size() > 0) {
        std::unordered_set<Node *> nodeSet;
        std::unordered_map<Input *, Node *> inputToNode;
        std::unordered_map<Output *, Node *> outputToNode;
        for (const auto &node : children) {
            node->lock();
            nodeSet.insert(node.get());
            for (const auto &input : node->inputs) {
                input->lock();
                inputToNode[input.get()] = node.get();
            }
            for (const auto &output : node->outputs) {
                output->lock();
                outputToNode[output.get()] = node.get();
            }
        }
        std::queue<Node *> queue;
        for (const auto &node : children) {
            if ([&] {
                    for (const auto &input : node->inputs) {
                        for (const auto &output : input->getConnections()) {
                            if (nodeSet.find(outputToNode[output.get()]) != nodeSet.end()) {
                                return false;
                            }
                        }
                    }
                    return true;
                }()) {
                queue.push(node.get());
            }
        }
        std::unordered_set<Node *> explored;
        std::unordered_map<Node *, unsigned int> orderMap;
        unsigned int increment = 0;
        while (!queue.empty()) {
            Node *node = queue.front();
            explored.insert(node);
            for (const auto &output : node->outputs) {
                for (const auto &input : output->getConnections()) {
                    Node *child = inputToNode[input.get()];
                    if (nodeSet.find(child) != nodeSet.end() && explored.find(child) == explored.end()) {
                        queue.push(child);
                    }
                }
            }
            orderMap[node] = increment;
            ++increment;
            queue.pop();
        }
        for (const auto &node : children) {
            for (const auto &output : node->outputs) {
                output->unlock();
            }
            for (const auto &input : node->inputs) {
                input->unlock();
            }
            node->unlock();
        }
        std::sort(children.begin(), children.end(), [&orderMap](const auto &a, const auto &b) {
            return orderMap[a.get()] < orderMap[b.get()];
        });
    }
    unlock();
}

void dsp::Node::disconnectAll() {
    lock();
    for (const auto &input : inputs) {
        input->disconnectAll();
    }
    for (const auto &output : outputs) {
        output->disconnectAll();
    }
    unlock();
}

void dsp::Node::process() {
    lock();
    for (const auto &input : inputs) {
        input->lock();
        input->processNoLock();
    }
    for (const auto &output : outputs) {
        output->lock();
        output->processNoLock();
    }
    if (active) {
        processNoLock();
    }
    for (const auto &output : outputs) {
        output->unlock();
    }
    for (const auto &input : inputs) {
        input->unlock();
    }
    unlock();
}

void dsp::Node::setNumInputChannelsNoLock(int numChannels) {
    numInputChannels = numChannels;
    for (const auto &input : inputs) {
        input->setNumChannels(numChannels);
    }
}

void dsp::Node::setNumOutputChannelsNoLock(int numChannels) {
    numOutputChannels = numChannels;
    for (const auto &output : outputs) {
        output->setNumChannels(numChannels);
    }
}

void dsp::Node::setNumSamplesNoLock(int numSamples) {
    this->numSamples = numSamples;
    oneOverNumSamples = 1.0 / numSamples;
    for (const auto &input : inputs) {
        input->setNumSamples(numSamples);
    }
    for (const auto &output : outputs) {
        output->setNumSamples(numSamples);
    }
    for (const auto &child : children) {
        child->setNumSamples(numSamples);
    }
}

void dsp::Node::setSampleRateNoLock(double sampleRate) {
    this->sampleRate = sampleRate;
    oneOverSampleRate = 1.0 / sampleRate;
    for (const auto &child : children) {
        child->setSampleRate(sampleRate);
    }
}
