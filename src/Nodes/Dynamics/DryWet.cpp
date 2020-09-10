#include "DryWet.h"

dsp::DryWet::DryWet(Type type, Space space)
        : Producer(type == Type::INTEGER ? Type::LOGARITHMIC : type, space)
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
    for (int channel = 0; channel < getNumChannels(); ++channel) {
        Sample *dryChannel = getDry()->getWrapper().getChannelPointer(channel);
        Sample *wetChannel = getWet()->getWrapper().getChannelPointer(channel);
        Sample *mixAmountChannel = getMixAmount()->getWrapper().getChannelPointer(channel);
        Sample *outputChannel = getOutput()->getWrapper().getChannelPointer(channel);
        for (int sample = 0; sample < getNumSamples(); ++sample) {
            Sample dry = dryChannel[sample];
            Sample wet = wetChannel[sample];
            Sample mixAmount = mixAmountChannel[sample];
            outputChannel[sample] = dry + mixAmount * (wet - dry);
        }
    }
}
