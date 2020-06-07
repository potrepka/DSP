#include "DryWetMix.h"

const std::size_t dsp::DryWetMix::DRY_SIGNAL = 0;
const std::size_t dsp::DryWetMix::WET_SIGNAL = 1;
const std::size_t dsp::DryWetMix::MIX = 2;

dsp::DryWetMix::DryWetMix() : Filter(Connection::Type::BIPOLAR) {
    pushInput(Connection::Type::BIPOLAR);
    pushInput(Connection::Type::UNIPOLAR);
}

std::shared_ptr<dsp::Unit::InputParameter> dsp::DryWetMix::getDrySignal() {
    return getInput(DRY_SIGNAL);
}

std::shared_ptr<dsp::Unit::InputParameter> dsp::DryWetMix::getWetSignal() {
    return getInput(WET_SIGNAL);
}

std::shared_ptr<dsp::Unit::InputParameter> dsp::DryWetMix::getMix() {
    return getInput(MIX);
}

void dsp::DryWetMix::process() {
    Filter::process();
    for (std::size_t i = 0; i < getNumChannels(); i++) {
        std::vector<DSP_FLOAT>& dryBuffer = getDrySignal()->getChannel(i)->getBuffer();
        std::vector<DSP_FLOAT>& wetBuffer = getWetSignal()->getChannel(i)->getBuffer();
        std::vector<DSP_FLOAT>& mixBuffer = getMix()->getChannel(i)->getBuffer();
        std::vector<DSP_FLOAT>& outputBuffer = getOutputSignal()->getChannel(i)->getBuffer();
        for (int k = 0; k < getBufferSize(); k++) {
            outputBuffer[k] = dryBuffer[k] + mixBuffer[k] * (wetBuffer[k] - dryBuffer[k]);
        }
    }
}
