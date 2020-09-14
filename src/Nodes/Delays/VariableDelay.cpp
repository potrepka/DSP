#include "VariableDelay.h"

dsp::VariableDelay::VariableDelay(Type type)
        : Transformer(type, type)
        , maxDelayTime(0.0)
        , buffer(std::make_shared<Buffer>(type))
        , resetTrigger(std::make_shared<Input>(Type::INTEGER))
        , delayTime(std::make_shared<Input>(Type::SECONDS))
        , decayTime(std::make_shared<Input>(Type::SECONDS))
        , feedbackSource(std::make_shared<Output>(type))
        , feedbackSink(std::make_shared<Input>(type))
        , feedbackProcessor(std::make_shared<Node>())
        , indexState(0) {
    getInputs().push_back(resetTrigger);
    getInputs().push_back(delayTime);
    getInputs().push_back(decayTime);
}

dsp::Sample dsp::VariableDelay::getMaxDelayTime() const {
    return maxDelayTime;
}

void dsp::VariableDelay::setMaxDelayTime(Sample seconds) {
    assert(seconds > 0.0);
    lock();
    maxDelayTime = seconds;
    indexState = 0;
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

std::shared_ptr<dsp::Output> dsp::VariableDelay::getFeedbackSource() const {
    return feedbackSource;
}

std::shared_ptr<dsp::Input> dsp::VariableDelay::getFeedbackSink() const {
    return feedbackSink;
}

std::shared_ptr<dsp::Node> dsp::VariableDelay::getFeedbackProcessor() const {
    return feedbackProcessor;
}

void dsp::VariableDelay::setNumInputChannelsNoLock(size_t numChannels) {
    Node::setNumInputChannelsNoLock(numChannels);
}

void dsp::VariableDelay::setNumOutputChannelsNoLock(size_t numChannels) {
    Node::setNumOutputChannelsNoLock(numChannels);
    buffer->setNumChannels(numChannels);
    feedbackSource->setNumChannels(numChannels);
    feedbackSink->setNumChannels(numChannels);
}

void dsp::VariableDelay::setSampleRateNoLock(double sampleRate) {
    Node::setSampleRateNoLock(sampleRate);
    indexState = 0;
    buffer->setNumSamples(getDelayBufferSize());
    feedbackSource->setNumSamples(1);
    feedbackSink->setNumSamples(1);
    feedbackProcessor->setNumSamples(1);
}

void dsp::VariableDelay::processNoLock() {
    feedbackSource->lock();
    feedbackSink->lock();
    feedbackProcessor->lock();
    if (buffer->getNumSamples() > 0) {
        std::vector<Sample> delayTimeClipped(getNumChannels());
        size_t writeIndex = indexState;
        for (size_t sample = 0; sample < getNumSamples(); ++sample) {
            for (size_t channel = 0; channel < getNumChannels(); ++channel) {
                Sample *bufferChannel = buffer->getWrapper().getChannelPointer(channel);
                Sample *inputChannel = getInput()->getWrapper().getChannelPointer(channel);
                Sample *resetTriggerChannel = getResetTrigger()->getWrapper().getChannelPointer(channel);
                Sample *delayTimeChannel = getDelayTime()->getWrapper().getChannelPointer(channel);
                Sample *feedbackSourceChannel = getFeedbackSource()->getWrapper().getChannelPointer(channel);
                Sample *outputChannel = getOutput()->getWrapper().getChannelPointer(channel);
                // RESET
                if (resetTriggerChannel[sample]) {
                    buffer->getWrapper().getSingleChannel(channel).clear();
                }
                // WRITE
                bufferChannel[writeIndex] = inputChannel[sample];
                // CLIP DELAY TIME
                delayTimeClipped[channel] = clip(delayTimeChannel[sample], 0.0, maxDelayTime);
                // READ
                if (delayTimeClipped[channel] == 0) {
                    outputChannel[sample] = bufferChannel[writeIndex];
                } else {
                    Sample readIndex = writeIndex - delayTimeClipped[channel] * getSampleRate();
                    if (readIndex < 0.0) {
                        readIndex += buffer->getNumSamples();
                    }
                    if (delayTimeClipped[channel] < getOneOverSampleRate()) {
                        outputChannel[sample] = linear(bufferChannel, buffer->getNumSamples(), readIndex);
                    } else {
                        outputChannel[sample] = hermite(bufferChannel, buffer->getNumSamples(), readIndex);
                    }
                }
                *feedbackSourceChannel = outputChannel[sample];
            }
            // PROCESS FEEDBACK
            if (feedbackProcessor->isActive()) {
                for (const auto &child : feedbackProcessor->getChildren()) {
                    child->process();
                }
            }
            feedbackSink->processNoLock();
            // ADD FEEDBACK
            for (size_t channel = 0; channel < getNumChannels(); ++channel) {
                Sample *bufferChannel = buffer->getWrapper().getChannelPointer(channel);
                Sample *decayTimeChannel = getDecayTime()->getWrapper().getChannelPointer(channel);
                Sample *feedbackSinkChannel = getFeedbackSink()->getWrapper().getChannelPointer(channel);
                Sample feedbackAmount = pow(0.001, delayTimeClipped[channel] / decayTimeChannel[sample]);
                bufferChannel[writeIndex] += *feedbackSinkChannel * feedbackAmount;
            }
            // INCREMENT INDEX
            ++writeIndex;
            if (writeIndex >= buffer->getNumSamples()) {
                writeIndex = 0;
            }
        }
        indexState += getNumSamples();
        indexState %= buffer->getNumSamples();
    }
    feedbackProcessor->unlock();
    feedbackSink->unlock();
    feedbackSource->unlock();
}

size_t dsp::VariableDelay::getDelayBufferSize() {
    return static_cast<size_t>(ceil(maxDelayTime * getSampleRate())) + 2;
}
