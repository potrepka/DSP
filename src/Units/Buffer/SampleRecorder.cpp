#include "SampleRecorder.h"

const unsigned int dsp::SampleRecorder::RESET_TRIGGER = 1;
const unsigned int dsp::SampleRecorder::GATE = 2;

dsp::SampleRecorder::SampleRecorder() : Consumer(Connection::Type::BIPOLAR) {
    pushInput(Connection::Type::BINARY);
}

std::vector<DSP_FLOAT> dsp::SampleRecorder::getSample(unsigned int channel) {
    lock();
    std::vector<DSP_FLOAT> sample = samples[channel];
    unlock();
    return sample;
}

std::shared_ptr<dsp::Unit::InputParameter> dsp::SampleRecorder::getResetTrigger() {
    return getInput(RESET_TRIGGER);
}

std::shared_ptr<dsp::Unit::InputParameter> dsp::SampleRecorder::getGate() {
    return getInput(GATE);
}

void dsp::SampleRecorder::setNumChannelsNoLock(unsigned int numChannels) {
    Unit::setNumChannelsNoLock(numChannels);
    samples.resize(numChannels);
    gatePrevious.resize(numChannels, 0);
}

void dsp::SampleRecorder::process() {
    Unit::process();
    for (unsigned int i = 0; i < getNumChannels(); i++) {
        std::vector<DSP_FLOAT> &inputBuffer = getInputSignal()->getChannel(i)->getBuffer();
        std::vector<DSP_FLOAT> &resetTriggerBuffer = getResetTrigger()->getChannel(i)->getBuffer();
        std::vector<DSP_FLOAT> &gateBuffer = getGate()->getChannel(i)->getBuffer();
        int bufferStart = 0;
        unsigned int size = static_cast<unsigned int>(samples[i].size());
        unsigned int index = size;
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
