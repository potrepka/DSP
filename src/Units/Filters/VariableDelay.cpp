#include "VariableDelay.h"

const unsigned int dsp::VariableDelay::DELAY_TIME = 1;

dsp::VariableDelay::VariableDelay() : Processor(Type::BIPOLAR, Type::BIPOLAR), maxDelayTime(0.0), writeIndex(0) {
    pushInput(Type::SECONDS);
    buffer = std::make_shared<Buffer>(0, 0, Type::BIPOLAR);
}

unsigned int dsp::VariableDelay::getMaxDelayTime() const {
    return maxDelayTime;
}

void dsp::VariableDelay::setMaxDelayTime(DSP_FLOAT seconds) {
    assert(seconds > 0);
    lock();
    maxDelayTime = seconds;
    buffer->setBufferSizeNoLock(getDelayBufferSize());
    writeIndex = 0;
    unlock();
}

std::shared_ptr<dsp::Unit::InputParameter> dsp::VariableDelay::getDelayTime() const {
    return getInput(DELAY_TIME);
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
        for (unsigned int i = 0; i < getNumChannels(); i++) {
            std::vector<DSP_FLOAT> &inputBuffer = getInputSignal()->getChannel(i)->getBuffer();
            std::vector<DSP_FLOAT> &outputBuffer = getOutputSignal()->getChannel(i)->getBuffer();
            std::vector<DSP_FLOAT> &delayTimeBuffer = getDelayTime()->getChannel(i)->getBuffer();
            for (unsigned int k = 0; k < getBufferSize(); k++) {
                const unsigned int index = (writeIndex + k) % buffer->getBufferSize();
                const DSP_FLOAT delayTime = clip(delayTimeBuffer[i], 0.0, maxDelayTime);
                DSP_FLOAT readIndex = static_cast<DSP_FLOAT>(index) - delayTime * getSampleRate();
                if (readIndex < 0.0) {
                    readIndex += buffer->getBufferSize();
                }
                buffer->getChannel(i)[index] = inputBuffer[k];
                outputBuffer[k] = hermite(buffer->getChannel(i), readIndex);
            }
        }
        writeIndex = (writeIndex + getBufferSize()) % buffer->getBufferSize();
    }
}

unsigned int dsp::VariableDelay::getDelayBufferSize() {
    return static_cast<unsigned int>(maxDelayTime * getSampleRate()) + 3;
}
