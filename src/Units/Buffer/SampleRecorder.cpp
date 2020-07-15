#include "SampleRecorder.h"

dsp::SampleRecorder::SampleRecorder(Type type)
        : Consumer(type)
        , resetTrigger(pushInput(Type::BINARY))
        , gate(pushInput(Type::BINARY)) {
    sample = std::make_shared<Buffer>(0, 0, type);
}

std::shared_ptr<dsp::Buffer> dsp::SampleRecorder::getSample() const {
    return sample;
}

std::shared_ptr<dsp::InputParameter> dsp::SampleRecorder::getResetTrigger() const {
    return resetTrigger;
}

std::shared_ptr<dsp::InputParameter> dsp::SampleRecorder::getGate() const {
    return gate;
}

void dsp::SampleRecorder::setNumChannelsNoLock(unsigned int numChannels) {
    Unit::setNumChannelsNoLock(numChannels);
    sample->setNumChannels(numChannels);
    gatePrevious.resize(numChannels, 0.0);
}

void dsp::SampleRecorder::process() {
    Unit::process();
    sample->lock();
    for (unsigned int i = 0; i < getNumChannels(); i++) {
        std::vector<DSP_FLOAT> &inputBuffer = getInputSignal()->getChannel(i)->getBuffer();
        std::vector<DSP_FLOAT> &resetTriggerBuffer = getResetTrigger()->getChannel(i)->getBuffer();
        std::vector<DSP_FLOAT> &gateBuffer = getGate()->getChannel(i)->getBuffer();
        int bufferStart = 0;
        unsigned int size = sample->getBufferSize();
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
        sample->setBufferSizeNoLock(size);
        for (unsigned int k = bufferStart; k < getBufferSize(); k++) {
            if (gateBuffer[k]) {
                sample->getChannel(i)[index] = inputBuffer[k];
                index++;
            }
        }
    }
    sample->unlock();
}
