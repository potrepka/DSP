#include "DryWetMix.h"

dsp::DryWetMix::DryWetMix(Type type, Space space)
        : Processor(type, type, space)
        , drySignal(getInputSignal())
        , wetSignal(pushInput(type, space))
        , mixAmount(pushInput(Type::UNIPOLAR, space)) {
    assert(type != Type::BINARY);
    assert(type != Type::INTEGER);
}

std::shared_ptr<dsp::InputParameter> dsp::DryWetMix::getDrySignal() const {
    return drySignal;
}

std::shared_ptr<dsp::InputParameter> dsp::DryWetMix::getWetSignal() const {
    return wetSignal;
}

std::shared_ptr<dsp::InputParameter> dsp::DryWetMix::getMixAmount() const {
    return mixAmount;
}

void dsp::DryWetMix::process() {
    Unit::process();
    for (unsigned int i = 0; i < getNumChannels(); i++) {
        std::vector<DSP_FLOAT> &dryBuffer = getDrySignal()->getChannel(i)->getBuffer();
        std::vector<DSP_FLOAT> &wetBuffer = getWetSignal()->getChannel(i)->getBuffer();
        std::vector<DSP_FLOAT> &mixBuffer = getMixAmount()->getChannel(i)->getBuffer();
        std::vector<DSP_FLOAT> &outputBuffer = getOutputSignal()->getChannel(i)->getBuffer();
        for (unsigned int k = 0; k < getBufferSize(); k++) {
            outputBuffer[k] = dryBuffer[k] + mixBuffer[k] * (wetBuffer[k] - dryBuffer[k]);
        }
    }
}
