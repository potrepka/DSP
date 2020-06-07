#include "ChannelMix.h"

const std::size_t dsp::ChannelMix::MIX = 1;

dsp::ChannelMix::ChannelMix() : Filter(Connection::Type::BIPOLAR) {
    pushInput(Connection::Type::UNIPOLAR);
}

std::shared_ptr<dsp::Unit::InputParameter> dsp::ChannelMix::getMix() {
    return getInput(MIX);
}

void dsp::ChannelMix::process() {
    Filter::process();
    if (getNumChannels() > 0) {
        if (buffer.size() != getBufferSize()) {
            buffer.resize(getBufferSize());
        }
        std::fill(buffer.begin(), buffer.end(), 0);
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
            for (int k = 0; k < getNumChannels(); k++) {
                outputBuffer[k] = inputBuffer[k] + mixBuffer[k] * (buffer[k] - inputBuffer[k]);
            }
        }
    }
}
