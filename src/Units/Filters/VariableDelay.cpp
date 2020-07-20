#include "VariableDelay.h"

dsp::VariableDelay::VariableDelay(Type type)
        : Processor(type, type)
        , delayTime(pushInput(Type::SECONDS))
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
            Array &outputBuffer = getOutputSignal()->getChannel(i)->getBuffer();
            Iterator inputIterator = inputBuffer.begin();
            Iterator delayTimeIterator = delayTimeBuffer.begin();
            Iterator outputIterator = outputBuffer.begin();
#if DSP_USE_VC
            Vector index = writeIndex + Vector::IndexesFromZero();
            Sample size = Vector::Size;
#else
            Sample index = writeIndex;
#endif
            while (outputIterator != outputBuffer.end()) {
#if DSP_USE_VC
                for (int k = 0; k < Vector::Size; ++k) {
                    buffer->getChannel(i)[static_cast<std::size_t>(index[k])] = (*inputIterator)[k];
                }
                const Vector delayTimeClipped = clip(*delayTimeIterator, 0.0, maxDelayTime);
                Vector readIndex = index - delayTimeClipped * getSampleRate();
                readIndex(readIndex < Vector::Zero()) += buffer->getBufferSize();
                *outputIterator = hermite(buffer->getChannel(i), readIndex);
                index += size;
                index(index >= buffer->getBufferSize()) -= buffer->getBufferSize();
#else
                buffer->getChannel(i)[static_cast<std::size_t>(index)] = *inputIterator;
                const Sample delayTimeClipped = clip(*delayTimeIterator, 0.0, maxDelayTime);
                Sample readIndex = index - delayTimeClipped * getSampleRate();
                if (readIndex < 0.0) {
                    readIndex += buffer->getBufferSize();
                }
                *outputIterator = hermite(buffer->getChannel(i), readIndex);
                index += 1.0;
                if (index >= buffer->getBufferSize()) {
                    index = 0;
                }
#endif
                ++inputIterator;
                ++delayTimeIterator;
                ++outputIterator;
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
