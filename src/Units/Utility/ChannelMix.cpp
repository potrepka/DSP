#include "ChannelMix.h"

dsp::ChannelMix::ChannelMix(Type type, Space space)
        : Processor(type, type, space)
        , mixAmount(pushInput(Type::UNIPOLAR, space)) {
    assert(type != Type::BINARY);
    assert(type != Type::INTEGER);
}

dsp::ChannelMix::Mode dsp::ChannelMix::getMode() const {
    return mode;
}

void dsp::ChannelMix::setMode(Mode mode) {
    lock();
    this->mode = mode;
    unlock();
}

std::shared_ptr<dsp::InputParameter> dsp::ChannelMix::getMixAmount() const {
    return mixAmount;
}

void dsp::ChannelMix::setBufferSizeNoLock(unsigned int bufferSize) {
    Unit::setBufferSizeNoLock(bufferSize);
    buffer.resize(bufferSize);
}

void dsp::ChannelMix::process() {
    Unit::process();
    if (getNumChannels() > 0) {
        std::fill(buffer.begin(), buffer.end(), 0.0);
        for (unsigned int i = 0; i < getNumChannels(); i++) {
            std::transform(buffer.begin(),
                           buffer.end(),
                           getInputSignal()->getChannel(i)->getBuffer().begin(),
                           buffer.begin(),
                           std::plus<Sample>());
        }
        std::transform(buffer.begin(), buffer.end(), buffer.begin(), [this](Sample x) { return x / getNumChannels(); });
        for (unsigned int i = 0; i < getNumChannels(); i++) {
            Array &inputBuffer = getInputSignal()->getChannel(i)->getBuffer();
            Array &mixBuffer = getMixAmount()->getChannel(i)->getBuffer();
            Array &outputBuffer = getOutputSignal()->getChannel(i)->getBuffer();
            for (unsigned int k = 0; k < getBufferSize(); k++) {
                Sample wet;
                switch (mode) {
                    case Mode::MID: wet = buffer[k] - inputBuffer[k]; break;
                    case Mode::SIDE: wet = -buffer[k]; break;
                }
                outputBuffer[k] = inputBuffer[k] + mixBuffer[k] * wet;
            }
        }
    }
}
