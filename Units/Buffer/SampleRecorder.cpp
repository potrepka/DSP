#include "SampleRecorder.h"

const std::size_t dsp::SampleRecorder::RESET_TRIGGER = 1;
const std::size_t dsp::SampleRecorder::GATE = 2;

dsp::SampleRecorder::SampleRecorder() : Consumer(Connection::Type::BIPOLAR) {
    pushInput(Connection::Type::BINARY);
}

void dsp::SampleRecorder::setNumChannels(std::size_t numChannels) {
    lock();
    setNumChannelsNoLock(numChannels);
    samples.resize(numChannels);
    gatePrevious.resize(numChannels, 0);
    unlock();
}

std::vector<DSP_FLOAT> dsp::SampleRecorder::getSample(std::size_t index) {
    lock();
    std::vector<DSP_FLOAT> sample = samples[index];
    unlock();
    return sample;
}

std::shared_ptr<dsp::Unit::InputParameter> dsp::SampleRecorder::getResetTrigger() {
    return getInput(RESET_TRIGGER);
}

std::shared_ptr<dsp::Unit::InputParameter> dsp::SampleRecorder::getGate() {
    return getInput(GATE);
}

void dsp::SampleRecorder::process() {
    Unit::process();
    for (std::size_t i = 0; i < getNumChannels(); i++) {
        std::vector<DSP_FLOAT> &inputBuffer = getInputSignal()->getChannel(i)->getBuffer();
        std::vector<DSP_FLOAT> &resetTriggerBuffer = getResetTrigger()->getChannel(i)->getBuffer();
        std::vector<DSP_FLOAT> &gateBuffer = getGate()->getChannel(i)->getBuffer();
        int bufferStart = 0;
        std::size_t size = samples[i].size();
        std::size_t index = size;
        for (unsigned int k = 0; k < getBufferSize(); k++) {
            if (resetTriggerBuffer[k]) {
                bufferStart = k;
                size = 0;
                index = 0;
            }
            if (gateBuffer[k]) {
                size++;
            }
        }
        samples[i].resize(size);
        for (unsigned int k = bufferStart; k < getBufferSize(); k++) {
            if (gateBuffer[k]) {
                samples[i][index] = inputBuffer[k];
                index++;
            }
        }
    }
}
