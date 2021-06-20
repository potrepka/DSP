#include "VariableDelay.h"

dsp::VariableDelay::VariableDelay(Type type)
        : Transformer(type, type)
        , maxDelayTime(0.0)
        , buffer(std::make_shared<Buffer>(type))
        , delayTime(std::make_shared<Input>(Type::SECONDS))
        , decayTime(std::make_shared<Input>(Type::SECONDS))
        , reset(std::make_shared<Input>(Type::BOOLEAN))
        , feedbackSource(std::make_shared<Output>(type))
        , feedbackSink(std::make_shared<Input>(type))
        , feedbackProcessor(std::make_shared<Node>())
        , writeIndex(0) {
    getInputs().push_back(delayTime);
    getInputs().push_back(decayTime);
    getInputs().push_back(reset);
}

dsp::Sample dsp::VariableDelay::getMaxDelayTime() const {
    return maxDelayTime;
}

void dsp::VariableDelay::setMaxDelayTime(Sample seconds) {
    DSP_ASSERT(seconds >= 0.0);
    lock();
    maxDelayTime = seconds;
    writeIndex = 0;
    buffer->setNumSamples(getDelayBufferSize());
    buffer->getWrapper().clear();
    unlock();
}

std::shared_ptr<dsp::Input> dsp::VariableDelay::getDelayTime() const {
    return delayTime;
}

std::shared_ptr<dsp::Input> dsp::VariableDelay::getDecayTime() const {
    return decayTime;
}

std::shared_ptr<dsp::Input> dsp::VariableDelay::getReset() const {
    return reset;
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

void dsp::VariableDelay::setNumSamplesNoLock(size_t numSamples) {
    Node::setNumSamplesNoLock(numSamples);
    feedbackSource->setNumSamples(1);
    feedbackSink->setNumSamples(1);
    feedbackProcessor->setNumSamples(1);
}

void dsp::VariableDelay::setSampleRateNoLock(double sampleRate) {
    Node::setSampleRateNoLock(sampleRate);
    writeIndex = 0;
    buffer->setNumSamples(getDelayBufferSize());
    buffer->getWrapper().clear();
}

void dsp::VariableDelay::processNoLock() {
    feedbackSource->lock();
    feedbackSink->lock();
    feedbackProcessor->lock();
    if (getBuffer()->getNumSamples() > 0) {
        std::vector<Sample> delayTimeClipped(getNumChannels());
        std::vector<Sample *> buffer(getNumChannels());
        std::vector<Sample *> input(getNumChannels());
        std::vector<Sample *> delayTime(getNumChannels());
        std::vector<Sample *> decayTime(getNumChannels());
        std::vector<Sample *> reset(getNumChannels());
        std::vector<Sample *> output(getNumChannels());
        std::vector<Sample *> feedbackSource(getNumChannels());
        std::vector<Sample *> feedbackSink(getNumChannels());
        for (size_t channel = 0; channel < getNumChannels(); ++channel) {
            buffer[channel] = getBuffer()->getWrapper().getChannelPointer(channel);
            input[channel] = getInput()->getWrapper().getChannelPointer(channel);
            delayTime[channel] = getDelayTime()->getWrapper().getChannelPointer(channel);
            decayTime[channel] = getDecayTime()->getWrapper().getChannelPointer(channel);
            reset[channel] = getReset()->getWrapper().getChannelPointer(channel);
            output[channel] = getOutput()->getWrapper().getChannelPointer(channel);
            feedbackSource[channel] = getFeedbackSource()->getWrapper().getChannelPointer(channel);
            feedbackSink[channel] = getFeedbackSink()->getWrapper().getChannelPointer(channel);
        }
        for (size_t sample = 0; sample < getNumSamples(); ++sample) {
            for (size_t channel = 0; channel < getNumChannels(); ++channel) {
                // RESET
                if (reset[channel][sample]) {
                    getBuffer()->getWrapper().getSingleChannel(channel).clear();
                }
                // WRITE
                buffer[channel][writeIndex] = input[channel][sample];
                // CLIP DELAY TIME
                delayTimeClipped[channel] = clip(delayTime[channel][sample], 0.0, maxDelayTime);
                // READ
                if (delayTimeClipped[channel] == 0.0) {
                    output[channel][sample] = buffer[channel][writeIndex];
                } else {
                    Sample readIndex = writeIndex - delayTimeClipped[channel] * getSampleRate();
                    if (readIndex < 0.0) {
                        readIndex += getBuffer()->getNumSamples();
                    }
                    if (delayTimeClipped[channel] <= getOneOverSampleRate()) {
                        output[channel][sample] = linear(buffer[channel], getBuffer()->getNumSamples(), readIndex);
                    } else {
                        output[channel][sample] = hermite(buffer[channel], getBuffer()->getNumSamples(), readIndex);
                    }
                }
                feedbackSource[channel][0] = output[channel][sample];
            }
            // PROCESS FEEDBACK
            if (feedbackProcessor->isActive()) {
                for (const auto &child : feedbackProcessor->getChildren()) {
                    child->process();
                }
            }
            getFeedbackSink()->processNoLock();
            // ADD FEEDBACK
            for (size_t channel = 0; channel < getNumChannels(); ++channel) {
                Sample feedbackAmount = pow(0.001, delayTimeClipped[channel] / decayTime[channel][sample]);
                if (isnan(feedbackAmount)) {
                    feedbackAmount = 0.0;
                }
                if (feedbackAmount > 1.0) {
                    feedbackAmount = 1.0;
                }
                buffer[channel][writeIndex] += feedbackSink[channel][0] * feedbackAmount;
            }
            // INCREMENT INDEX
            ++writeIndex;
            if (writeIndex == getBuffer()->getNumSamples()) {
                writeIndex = 0;
            }
        }
    }
    feedbackProcessor->unlock();
    feedbackSink->unlock();
    feedbackSource->unlock();
}

const std::shared_ptr<dsp::Buffer> dsp::VariableDelay::getBuffer() const {
    return buffer;
}

size_t dsp::VariableDelay::getDelayBufferSize() {
    return static_cast<size_t>(ceil(maxDelayTime * getSampleRate())) + 2;
}
