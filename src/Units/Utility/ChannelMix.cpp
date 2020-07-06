#include "ChannelMix.h"

const unsigned int dsp::ChannelMix::MIX = 1;

dsp::ChannelMix::ChannelMix(Connection::Type type, Connection::Space space) : Processor(type, type, space) {
    assert(type != Connection::Type::BINARY);
    assert(type != Connection::Type::INTEGER);
    pushInput(Connection::Type::UNIPOLAR, space);
}

dsp::ChannelMix::Mode dsp::ChannelMix::getMode() {
    return mode;
}

void dsp::ChannelMix::setMode(Mode mode) {
    this->mode = mode;
}

std::shared_ptr<dsp::Unit::InputParameter> dsp::ChannelMix::getMix() {
    return getInput(MIX);
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
                           std::plus<DSP_FLOAT>());
        }
        std::transform(
                buffer.begin(), buffer.end(), buffer.begin(), [this](DSP_FLOAT x) { return x / getNumChannels(); });
        for (unsigned int i = 0; i < getNumChannels(); i++) {
            std::vector<DSP_FLOAT> &inputBuffer = getInputSignal()->getChannel(i)->getBuffer();
            std::vector<DSP_FLOAT> &outputBuffer = getOutputSignal()->getChannel(i)->getBuffer();
            std::vector<DSP_FLOAT> &mixBuffer = getMix()->getChannel(i)->getBuffer();
            for (unsigned int k = 0; k < getBufferSize(); k++) {
                DSP_FLOAT wet;
                switch (mode) {
                    case Mode::MID: wet = buffer[k] - inputBuffer[k]; break;
                    case Mode::SIDE: wet = -buffer[k]; break;
                }
                outputBuffer[k] = inputBuffer[k] + mixBuffer[k] * wet;
            }
        }
    }
}
