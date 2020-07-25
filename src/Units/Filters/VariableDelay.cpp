#include "VariableDelay.h"

dsp::VariableDelay::VariableDelay(Type type)
        : Processor(type, type)
        , delayTime(pushInput(Type::SECONDS))
        , feedback(pushInput(Type::UNIPOLAR))
        , maxDelayTime(0.0)
        , writeIndex(0) {
    buffer = std::make_shared<Buffer>(0, 0, type);
}

unsigned int dsp::VariableDelay::getMaxDelayTime() const {
    return maxDelayTime;
}

void dsp::VariableDelay::setMaxDelayTime(Sample seconds) {
    assert(seconds > 0.0);
    lock();
    maxDelayTime = seconds;
    buffer->setBufferSizeNoLock(getDelayBufferSize());
    writeIndex = 0;
    unlock();
}

std::shared_ptr<dsp::InputParameter> dsp::VariableDelay::getDelayTime() const {
    return delayTime;
}

std::shared_ptr<dsp::InputParameter> dsp::VariableDelay::getFeedback() const {
    return feedback;
}

void dsp::VariableDelay::setSampleRateNoLock(unsigned int sampleRate) {
    Unit::setSampleRateNoLock(sampleRate);
    buffer->setBufferSizeNoLock(getDelayBufferSize());
    writeIndex = 0;
}

void dsp::VariableDelay::setNumChannelsNoLock(unsigned int numChannels) {
    Unit::setNumChannelsNoLock(numChannels);
    buffer->setNumChannelsNoLock(numChannels);
}

void dsp::VariableDelay::process() {
    Unit::process();
    if (buffer->getBufferSize() > 0) {
        for (unsigned int i = 0; i < getNumChannels(); ++i) {
            Array &inputBuffer = getInputSignal()->getChannel(i)->getBuffer();
            Array &delayTimeBuffer = getDelayTime()->getChannel(i)->getBuffer();
            Array &feedbackBuffer = getFeedback()->getChannel(i)->getBuffer();
            Array &outputBuffer = getOutputSignal()->getChannel(i)->getBuffer();
            Sample index = writeIndex;
            for (unsigned int k = 0; k < getBufferSize(); ++k) {
                buffer->getChannel(i)[static_cast<std::size_t>(index)] = inputBuffer[k];
                const Sample delayTimeClipped = clip(delayTimeBuffer[k], 0.0, maxDelayTime);
                Sample readIndex = index - delayTimeClipped * getSampleRate();
                if (readIndex < 0.0) {
                    readIndex += buffer->getBufferSize();
                }
                outputBuffer[k] = hermite(buffer->getChannel(i), readIndex);
                if (std::isnan(outputBuffer[k])) {
                    outputBuffer[k] = 0.0;
                }
                if (delayTimeClipped >= getOneOverSampleRate()) {
                    buffer->getChannel(i)[static_cast<std::size_t>(index)] += outputBuffer[k] * feedbackBuffer[k];
                }
                index += 1.0;
                if (index >= buffer->getBufferSize()) {
                    index = 0;
                }
            }
        }
        writeIndex += getBufferSize();
        writeIndex %= buffer->getBufferSize();
    }
}

unsigned int dsp::VariableDelay::getDelayBufferSize() {
#if DSP_USE_VC
    return static_cast<unsigned int>(ceil(maxDelayTime * getSampleRate())) + 2 + Vector::Size;
#else
    return static_cast<unsigned int>(ceil(maxDelayTime * getSampleRate())) + 2;
#endif
}
