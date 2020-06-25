#include "ChannelMix.h"

const std::size_t dsp::ChannelMix::MIX = 1;

dsp::ChannelMix::ChannelMix() : Processor(Connection::Type::BIPOLAR, Connection::Type::BIPOLAR) {
    pushInput(Connection::Type::UNIPOLAR);
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
        for (std::size_t i = 0; i < getNumChannels(); i++) {
            std::transform(buffer.begin(),
                           buffer.end(),
                           getInputSignal()->getChannel(i)->getBuffer().begin(),
                           buffer.begin(),
                           std::plus<DSP_FLOAT>());
        }
        std::transform(
                buffer.begin(), buffer.end(), buffer.begin(), [this](DSP_FLOAT x) { return x / getNumChannels(); });
        for (std::size_t i = 0; i < getNumChannels(); i++) {
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