#include "VariableDelay.h"

dsp::VariableDelay::VariableDelay(Type type)
        : Transformer(type, type)
        , maxDelayTime(0.0)
        , resetTrigger(std::make_shared<Input>(Type::INTEGER))
        , delayTime(std::make_shared<Input>(Type::SECONDS))
        , decayTime(std::make_shared<Input>(Type::SECONDS))
        , index(0)
        , buffer(std::make_shared<Buffer>(type)) {
    getInputs().push_back(resetTrigger);
    getInputs().push_back(delayTime);
    getInputs().push_back(decayTime);
}

unsigned int dsp::VariableDelay::getMaxDelayTime() const {
    return maxDelayTime;
}

void dsp::VariableDelay::setMaxDelayTime(Sample seconds) {
    assert(seconds > 0.0);
    lock();
    maxDelayTime = seconds;
    index = 0;
    buffer->setNumSamples(getDelayBufferSize());
    unlock();
}

std::shared_ptr<dsp::Input> dsp::VariableDelay::getResetTrigger() const {
    return resetTrigger;
}

std::shared_ptr<dsp::Input> dsp::VariableDelay::getDelayTime() const {
    return delayTime;
}

std::shared_ptr<dsp::Input> dsp::VariableDelay::getDecayTime() const {
    return decayTime;
}

void dsp::VariableDelay::setNumOutputChannelsNoLock(int numChannels) {
    Node::setNumOutputChannelsNoLock(numChannels);
    buffer->setNumChannels(numChannels);
}

void dsp::VariableDelay::setSampleRateNoLock(double sampleRate) {
    Node::setSampleRateNoLock(sampleRate);
    index = 0;
    buffer->setNumSamples(getDelayBufferSize());
}

void dsp::VariableDelay::processNoLock() {
    if (buffer->getNumSamples() > 0) {
        for (int channel = 0; channel < getNumChannels(); ++channel) {
            Sample *inputChannel = getInput()->getWrapper().getChannelPointer(channel);
            Sample *resetTriggerChannel = getResetTrigger()->getWrapper().getChannelPointer(channel);
            Sample *delayTimeChannel = getDelayTime()->getWrapper().getChannelPointer(channel);
            Sample *decayTimeChannel = getDecayTime()->getWrapper().getChannelPointer(channel);
            Sample *outputChannel = getOutput()->getWrapper().getChannelPointer(channel);
            Sample *bufferChannel = buffer->getWrapper().getChannelPointer(channel);
            int writeIndex = index;
            for (int sample = 0; sample < getNumSamples(); ++sample) {
                if (resetTriggerChannel[sample]) {
                    buffer->getWrapper().getSingleChannelWrapper(channel).clear();
                }
                // WRITING
                bufferChannel[writeIndex] = inputChannel[sample];
                // READING
                Sample delayTimeClipped = clip(delayTimeChannel[sample], 0.0, maxDelayTime);
                if (delayTimeClipped < getOneOverSampleRate()) {
                    delayTimeClipped = 0.0;
                }
                Sample readIndex = writeIndex - delayTimeClipped * getSampleRate();
                if (readIndex < 0.0) {
                    readIndex += buffer->getNumSamples();
                }
                outputChannel[sample] = hermite(bufferChannel, buffer->getNumSamples(), readIndex);
                if (std::isinf(outputChannel[sample]) || std::isnan(outputChannel[sample])) {
                    outputChannel[sample] = 0.0;
                }
                // FEEDBACK
                if (delayTimeClipped >= getOneOverSampleRate()) {
                    Sample feedback = pow(0.001, delayTimeClipped / decayTimeChannel[sample]);
                    bufferChannel[writeIndex] += outputChannel[sample] * feedback;
                }
                // INCREMENT
                ++writeIndex;
                if (writeIndex >= buffer->getNumSamples()) {
                    writeIndex = 0;
                }
            }
        }
        index += getNumSamples();
        index %= buffer->getNumSamples();
    }
}

int dsp::VariableDelay::getDelayBufferSize() {
    return static_cast<int>(ceil(maxDelayTime * getSampleRate())) + 2;
}
