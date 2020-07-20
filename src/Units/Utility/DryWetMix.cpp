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
    for (unsigned int i = 0; i < getNumChannels(); ++i) {
        Array &dryBuffer = getDrySignal()->getChannel(i)->getBuffer();
        Array &wetBuffer = getWetSignal()->getChannel(i)->getBuffer();
        Array &mixAmountBuffer = getMixAmount()->getChannel(i)->getBuffer();
        Array &outputBuffer = getOutputSignal()->getChannel(i)->getBuffer();
        Iterator dryIterator = dryBuffer.begin();
        Iterator wetIterator = wetBuffer.begin();
        Iterator mixAmountIterator = mixAmountBuffer.begin();
        Iterator outputIterator = outputBuffer.begin();
        while (outputIterator != outputBuffer.end()) {
#if DSP_USE_VC
            Vector d = *dryIterator;
            Vector w = *wetIterator;
#else
            Sample d = *dryIterator;
            Sample w = *wetIterator;
#endif
            *outputIterator = d + *mixAmountIterator * (w - d);
            ++dryIterator;
            ++wetIterator;
            ++mixAmountIterator;
            ++outputIterator;
        }
    }
}
