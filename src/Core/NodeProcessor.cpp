#include "NodeProcessor.h"

dsp::NodeProcessor::NodeProcessor(size_t numInputChannels,
                                  size_t numOutputChannels,
                                  size_t numSamples,
                                  double sampleRate)
        : audioInput(std::make_shared<Output>(Type::RATIO, Space::TIME, 0.0, 0.0, numInputChannels, numSamples))
        , audioOutput(std::make_shared<Input>(Type::RATIO, Space::TIME, 0.0, 0.0, numOutputChannels, numSamples))
        , audioInputClipping(
                  std::make_shared<Output>(Type::BOOLEAN, Space::TIME, 0.0, 0.0, numInputChannels, numSamples))
        , audioOutputClipping(
                  std::make_shared<Output>(Type::BOOLEAN, Space::TIME, 0.0, 0.0, numOutputChannels, numSamples))
        , active(true)
        , numInputChannels(numInputChannels)
        , numOutputChannels(numOutputChannels)
        , numSamples(numSamples)
        , sampleRate(sampleRate)
        , defaultNode(std::make_shared<Node>())
        , inputMessages(std::make_shared<dsp::MidiBuffer>())
        , outputMessages(std::make_shared<dsp::MidiBuffer>()) {
    defaultNode->setNumSamples(numSamples);
    defaultNode->setSampleRate(sampleRate);
    nodes.push_back(defaultNode);
}

dsp::NodeProcessor::~NodeProcessor() {
    nodes.clear();
}

bool dsp::NodeProcessor::isActive() const {
    return active;
}

void dsp::NodeProcessor::setActive(bool active) {
    lock();
    this->active = active;
    unlock();
}

std::shared_ptr<dsp::Output> dsp::NodeProcessor::getAudioInput() const {
    return audioInput;
}

std::shared_ptr<dsp::Input> dsp::NodeProcessor::getAudioOutput() const {
    return audioOutput;
}

std::shared_ptr<dsp::Output> dsp::NodeProcessor::getAudioInputClipping() const {
    return audioInputClipping;
}

std::shared_ptr<dsp::Output> dsp::NodeProcessor::getAudioOutputClipping() const {
    return audioOutputClipping;
}

size_t dsp::NodeProcessor::getNumInputChannels() const {
    return numInputChannels;
}

void dsp::NodeProcessor::setNumInputChannels(size_t numChannels) {
    lock();
    numInputChannels = numChannels;
    audioInput->setSize(numChannels, getNumSamples());
    audioInputClipping->setSize(numChannels, getNumSamples());
    unlock();
}

size_t dsp::NodeProcessor::getNumOutputChannels() const {
    return numOutputChannels;
}

void dsp::NodeProcessor::setNumOutputChannels(size_t numChannels) {
    lock();
    numOutputChannels = numChannels;
    audioOutput->setNumChannels(numChannels);
    audioOutputClipping->setNumChannels(numChannels);
    unlock();
}

size_t dsp::NodeProcessor::getNumSamples() const {
    return numSamples;
}

void dsp::NodeProcessor::setNumSamples(size_t numSamples) {
    lock();
    this->numSamples = numSamples;
    audioInput->setNumSamples(numSamples);
    audioOutput->setNumSamples(numSamples);
    audioInputClipping->setNumSamples(numSamples);
    audioOutputClipping->setNumSamples(numSamples);
    for (const auto &node : nodes) {
        node->setNumSamples(numSamples);
    }
    unlock();
}

double dsp::NodeProcessor::getSampleRate() const {
    return sampleRate;
}

void dsp::NodeProcessor::setSampleRate(double sampleRate) {
    DSP_ASSERT(sampleRate >= 0.0);
    lock();
    this->sampleRate = sampleRate;
    for (const auto &node : nodes) {
        node->setSampleRate(sampleRate);
    }
    unlock();
}

void dsp::NodeProcessor::setInputSize(size_t numChannels, size_t numSamples) {
    lock();
    numInputChannels = numChannels;
    this->numSamples = numSamples;
    audioInput->setSize(numChannels, numSamples);
    audioInputClipping->setSize(numChannels, numSamples);
    for (const auto &node : nodes) {
        node->setNumSamples(numSamples);
    }
    unlock();
}

void dsp::NodeProcessor::setOutputSize(size_t numChannels, size_t numSamples) {
    lock();
    numOutputChannels = numChannels;
    this->numSamples = numSamples;
    audioOutput->setSize(numChannels, numSamples);
    audioOutputClipping->setSize(numChannels, numSamples);
    for (const auto &node : nodes) {
        node->setNumSamples(numSamples);
    }
    unlock();
}

std::shared_ptr<dsp::Node> dsp::NodeProcessor::getDefaultNode() const {
    return defaultNode;
}

std::vector<std::shared_ptr<dsp::Node>> &dsp::NodeProcessor::getNodes() {
    return nodes;
}

std::shared_ptr<dsp::MidiBuffer> dsp::NodeProcessor::getInputMessages() const {
    return inputMessages;
}

std::shared_ptr<dsp::MidiBuffer> dsp::NodeProcessor::getOutputMessages() const {
    return outputMessages;
}

template <typename T>
void dsp::NodeProcessor::process(AudioBuffer<T> &audioBuffer, MidiBuffer &midiBuffer) {
    lock();

    if (active) {
        audioInput->lock();
        audioInput->prepareNoLock();
        audioInput->unlock();

        for (size_t channel = 0; channel < audioInput->getNumChannels(); ++channel) {
            auto *bufferChannel = audioBuffer.getReadPointer(channel);
            auto *inputChannel = audioInput->getWrapper().getChannelPointer(channel);
            for (auto sample = 0; sample < audioInput->getNumSamples(); ++sample) {
                inputChannel[sample] = static_cast<Sample>(bufferChannel[sample]);
            }
        }

        processClipping(audioInput->getWrapper(), audioInputClipping->getWrapper());

        inputMessages->clear();
        inputMessages->addEvents(midiBuffer, 0, audioBuffer.getNumSamples(), 0);
        outputMessages->clear();

        for (const auto &node : nodes) {
            node->process();
        }

        midiBuffer.clear();
        midiBuffer.addEvents(*outputMessages, 0, audioBuffer.getNumSamples(), 0);

        audioOutput->lock();
        audioOutput->prepareNoLock();
        audioOutput->processNoLock();
        audioOutput->unlock();

        processClipping(audioOutput->getWrapper(), audioOutputClipping->getWrapper());

        for (size_t channel = 0; channel < audioOutput->getNumChannels(); ++channel) {
            auto *outputChannel = audioOutput->getWrapper().getChannelPointer(channel);
            auto *bufferChannel = audioBuffer.getWritePointer(channel);
            for (auto sample = 0; sample < audioOutput->getNumSamples(); ++sample) {
                bufferChannel[sample] = static_cast<T>(clip(outputChannel[sample], -1.0, 1.0));
            }
        }
    } else {
        audioInput->getWrapper().clear();
        audioInputClipping->getWrapper().clear();
        audioOutput->getWrapper().clear();
        audioOutputClipping->getWrapper().clear();
        audioBuffer.clear();
    }

    unlock();
}

template void dsp::NodeProcessor::process(AudioBuffer<float> &audioBuffer, MidiBuffer &midiBuffer);
template void dsp::NodeProcessor::process(AudioBuffer<double> &audioBuffer, MidiBuffer &midiBuffer);

void dsp::NodeProcessor::processClipping(Wrapper &audioWrapper, Wrapper &audioClippingWrapper) {
    for (size_t channel = 0; channel < audioWrapper.getNumChannels(); ++channel) {
        auto *audioChannel = audioWrapper.getChannelPointer(channel);
        auto *audioClippingChannel = audioClippingWrapper.getChannelPointer(channel);
        for (auto sample = 0; sample < audioWrapper.getNumSamples(); ++sample) {
            audioClippingChannel[sample] = abs(audioChannel[sample]) > 1.0;
        }
    }
}
