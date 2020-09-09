#include "NodeProcessor.h"

dsp::NodeProcessor::NodeProcessor(int numInputChannels, int numOutputChannels, int numSamples, double sampleRate)
        : audioInput(std::make_shared<Output>(Type::RATIO, Space::TIME, numInputChannels, numSamples))
        , audioOutput(std::make_shared<Input>(Type::RATIO, Space::TIME, numOutputChannels, numSamples))
        , audioInputClipping(std::make_shared<Output>(Type::RATIO, Space::TIME, numInputChannels, numSamples))
        , audioOutputClipping(std::make_shared<Output>(Type::RATIO, Space::TIME, numOutputChannels, numSamples))
        , numInputChannels(numInputChannels)
        , numOutputChannels(numOutputChannels)
        , numSamples(numSamples)
        , sampleRate(sampleRate)
        , inputMessages(std::make_shared<dsp::MidiData>())
        , outputMessages(std::make_shared<dsp::MidiData>()) {}

dsp::NodeProcessor::~NodeProcessor() {
    nodes.clear();
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

int dsp::NodeProcessor::getNumInputChannels() const {
    return numInputChannels;
}

void dsp::NodeProcessor::setNumInputChannels(int numChannels) {
    DSP_ASSERT(numChannels >= 0);
    lock();
    numInputChannels = numChannels;
    audioInput->setSize(numChannels, getNumSamples());
    audioInputClipping->setSize(numChannels, getNumSamples());
    unlock();
}

int dsp::NodeProcessor::getNumOutputChannels() const {
    return numOutputChannels;
}

void dsp::NodeProcessor::setNumOutputChannels(int numChannels) {
    DSP_ASSERT(numChannels >= 0);
    lock();
    numOutputChannels = numChannels;
    audioOutput->setNumChannels(numChannels);
    audioOutputClipping->setNumChannels(numChannels);
    unlock();
}

int dsp::NodeProcessor::getNumSamples() const {
    return numSamples;
}

void dsp::NodeProcessor::setNumSamples(int numSamples) {
    DSP_ASSERT(numSamples >= 0);
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
    DSP_ASSERT(sampleRate >= 0);
    lock();
    this->sampleRate = sampleRate;
    for (const auto &node : nodes) {
        node->setSampleRate(sampleRate);
    }
    unlock();
}

void dsp::NodeProcessor::setInputSize(int numChannels, int numSamples) {
    DSP_ASSERT(numChannels >= 0 && numSamples >= 0);
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

void dsp::NodeProcessor::setOutputSize(int numChannels, int numSamples) {
    DSP_ASSERT(numChannels >= 0 && numSamples >= 0);
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

std::vector<std::shared_ptr<dsp::Node>> &dsp::NodeProcessor::getNodes() {
    return nodes;
}

std::shared_ptr<dsp::MidiData> dsp::NodeProcessor::getInputMessages() const {
    return inputMessages;
}

std::shared_ptr<dsp::MidiData> dsp::NodeProcessor::getOutputMessages() const {
    return outputMessages;
}

template <typename T>
void dsp::NodeProcessor::process(AudioData<T> &audioData, MidiData &midiData) {
    lock();

    audioInput->lock();
    audioInput->processNoLock();
    audioInput->unlock();

    for (int channel = 0; channel < audioInput->getNumChannels(); ++channel) {
        auto *dataChannel = audioData.getReadPointer(channel);
        auto *inputChannel = audioInput->getBlock().getChannelPointer(channel);
        for (auto sample = 0; sample < audioInput->getNumSamples(); ++sample) {
            inputChannel[sample] = static_cast<Sample>(dataChannel[sample]);
        }
    }
    processClipping(audioInput->getBlock(), audioInputClipping->getBlock());

    inputMessages->clear();
    inputMessages->addEvents(midiData, 0, static_cast<int>(audioData.getNumSamples()), 0);
    outputMessages->clear();
    for (const auto &node : nodes) {
        node->process();
    }
    midiData.clear();
    midiData.addEvents(*outputMessages, 0, static_cast<int>(audioData.getNumSamples()), 0);

    audioOutput->lock();
    audioOutput->processNoLock();
    audioOutput->unlock();

    for (int channel = 0; channel < audioOutput->getNumChannels(); ++channel) {
        auto *outputChannel = audioOutput->getBlock().getChannelPointer(channel);
        auto *dataChannel = audioData.getWritePointer(channel);
        for (auto sample = 0; sample < audioOutput->getNumSamples(); ++sample) {
            dataChannel[sample] = static_cast<T>(outputChannel[sample]);
        }
    }
    processClipping(audioOutput->getBlock(), audioOutputClipping->getBlock());

    unlock();
}

template void dsp::NodeProcessor::process(AudioData<float> &audioData, MidiData &midiData);
template void dsp::NodeProcessor::process(AudioData<double> &audioData, MidiData &midiData);

void dsp::NodeProcessor::processClipping(Block &audioBlock, Block &audioClippingBlock) {
    for (int channel = 0; channel < audioBlock.getNumChannels(); ++channel) {
        auto *audioChannel = audioBlock.getChannelPointer(channel);
        auto *audioClippingChannel = audioClippingBlock.getChannelPointer(channel);
        for (auto sample = 0; sample < audioBlock.getNumSamples(); ++sample) {
            audioClippingChannel[sample] = abs(audioChannel[sample]) > 1.0;
        }
    }
}
