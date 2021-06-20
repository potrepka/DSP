#include "DryWet.h"

dsp::DryWet::DryWet(Type type, Space space)
        : Producer(type, space)
        , dry(std::make_shared<Input>(type, space))
        , wet(std::make_shared<Input>(type, space))
        , mixAmount(std::make_shared<Input>(Type::RATIO, space, 1.0, 0.0))
        , a(getOutput())
        , b(std::make_shared<Output>(type, space)) {
    getInputs().push_back(dry);
    getInputs().push_back(wet);
    getInputs().push_back(mixAmount);
    getOutputs().push_back(b);
}

std::shared_ptr<dsp::Input> dsp::DryWet::getDry() const {
    return dry;
}

std::shared_ptr<dsp::Input> dsp::DryWet::getWet() const {
    return wet;
}

std::shared_ptr<dsp::Input> dsp::DryWet::getMixAmount() const {
    return mixAmount;
}

std::shared_ptr<dsp::Output> dsp::DryWet::getA() const {
    return a;
}

std::shared_ptr<dsp::Output> dsp::DryWet::getB() const {
    return b;
}

void dsp::DryWet::processNoLock() {
    for (size_t channel = 0; channel < getNumChannels(); ++channel) {
        Sample *dryChannel = getDry()->getWrapper().getChannelPointer(channel);
        Sample *wetChannel = getWet()->getWrapper().getChannelPointer(channel);
        Sample *mixAmountChannel = getMixAmount()->getWrapper().getChannelPointer(channel);
        Sample *aChannel = getA()->getWrapper().getChannelPointer(channel);
        Sample *bChannel = getB()->getWrapper().getChannelPointer(channel);
        for (size_t sample = 0; sample < getNumSamples(); ++sample) {
            Sample &dry = dryChannel[sample];
            Sample &wet = wetChannel[sample];
            Sample &mixAmount = mixAmountChannel[sample];
            Sample &a = aChannel[sample];
            Sample &b = bChannel[sample];
            Sample difference = mixAmount * (wet - dry);
            a = dry + difference;
            b = wet - difference;
        }
    }
}
