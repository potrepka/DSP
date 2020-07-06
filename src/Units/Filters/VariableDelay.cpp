#include "VariableDelay.h"

const unsigned int dsp::VariableDelay::DELAY_TIME = 1;

dsp::VariableDelay::VariableDelay()
        : Processor(Connection::Type::BIPOLAR, Connection::Type::BIPOLAR), maxDelayTime(0.0), writeIndex(0) {
    pushInput(Connection::Type::SECONDS);
}

unsigned int dsp::VariableDelay::getMaxDelayTime() {
    return maxDelayTime;
}

void dsp::VariableDelay::setMaxDelayTime(DSP_FLOAT seconds) {
    lock();
    maxDelayTime = seconds;
    resizeBuffers();
    unlock();
}

std::shared_ptr<dsp::Unit::InputParameter> dsp::VariableDelay::getDelayTime() {
    return getInput(DELAY_TIME);
}

void dsp::VariableDelay::setSampleRateNoLock(unsigned int sampleRate) {
    Unit::setSampleRateNoLock(sampleRate);
    resizeBuffers();
}

void dsp::VariableDelay::setNumChannelsNoLock(unsigned int numChannels) {
    Unit::setNumChannelsNoLock(numChannels);
    buffers.resize(numChannels, std::vector<DSP_FLOAT>(getDelayBufferSize(), 0.0));
}

void dsp::VariableDelay::process() {
    Unit::process();
    for (unsigned int i = 0; i < getNumChannels(); i++) {
        std::vector<DSP_FLOAT> &inputBuffer = getInputSignal()->getChannel(i)->getBuffer();
        std::vector<DSP_FLOAT> &outputBuffer = getOutputSignal()->getChannel(i)->getBuffer();
        std::vector<DSP_FLOAT> &delayTimeBuffer = getDelayTime()->getChannel(i)->getBuffer();
        for (unsigned int k = 0; k < getBufferSize(); k++) {
            const unsigned int index = (writeIndex + k) % buffers[i].size();
            const DSP_FLOAT delayTime = clip(delayTimeBuffer[i], 0.0, maxDelayTime);
            DSP_FLOAT readIndex = static_cast<DSP_FLOAT>(index) - delayTime * getSampleRate();
            if (readIndex < 0.0) {
                readIndex += buffers[i].size();
            }
            buffers[i][index] = inputBuffer[k];
            outputBuffer[k] = hermite(buffers[i], readIndex);
        }
    }
    writeIndex = (writeIndex + getBufferSize()) % getDelayBufferSize();
}

unsigned int dsp::VariableDelay::getDelayBufferSize() {
    return static_cast<unsigned int>(maxDelayTime * getSampleRate()) + 3;
}

void dsp::VariableDelay::resizeBuffers() {
    unsigned int samples = getDelayBufferSize();
    for (unsigned int i = 0; i < getNumChannels(); i++) {
        buffers[i].resize(samples);
        std::fill(buffers[i].begin(), buffers[i].end(), 0.0);
    }
    writeIndex = 0;
}
