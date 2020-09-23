#include "DryWet.h"

dsp::DryWet::DryWet(Type type, Space space)
        : Producer(type, space)
        , dry(std::make_shared<Input>(type, space))
        , wet(std::make_shared<Input>(type, space))
        , mixAmount(std::make_shared<Input>(Type::RATIO, space)) {
    getInputs().push_back(dry);
    getInputs().push_back(wet);
    getInputs().push_back(mixAmount);
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

void dsp::DryWet::processNoLock() {
    for (size_t channel = 0; channel < getNumChannels(); ++channel) {
        Sample *dryChannel = getDry()->getWrapper().getChannelPointer(channel);
        Sample *wetChannel = getWet()->getWrapper().getChannelPointer(channel);
        Sample *mixAmountChannel = getMixAmount()->getWrapper().getChannelPointer(channel);
        Sample *outputChannel = getOutput()->getWrapper().getChannelPointer(channel);
        for (size_t sample = 0; sample < getNumSamples(); ++sample) {
            Sample &dry = dryChannel[sample];
            Sample &wet = wetChannel[sample];
            Sample &output = outputChannel[sample];
            Sample amount = clip(mixAmountChannel[sample], 0.0, 1.0);
            output = dry + amount * (wet - dry);
        }
    }
}
