#include "Spread.h"

dsp::Spread::Spread(Type type, Space space)
        : Transformer(type, type, space)
        , spread(std::make_shared<Input>(type, space))
        , mode(std::make_shared<Input>(Type::INTEGER, space, Mode::MAX)) {
    getInputs().push_back(spread);
    getInputs().push_back(mode);
}

std::shared_ptr<dsp::Input> dsp::Spread::getSpread() const {
    return spread;
}

std::shared_ptr<dsp::Input> dsp::Spread::getMode() const {
    return mode;
}

void dsp::Spread::processNoLock() {
    size_t numChannelsMinusOne = getNumChannels() - 1;
    for (size_t channel = 0; channel < getNumChannels(); ++channel) {
        Sample *inputChannel = getInput()->getWrapper().getChannelPointer(channel);
        Sample *spreadChannel = getSpread()->getWrapper().getChannelPointer(channel);
        Sample *modeChannel = getMode()->getWrapper().getChannelPointer(channel);
        Sample *outputChannel = getOutput()->getWrapper().getChannelPointer(channel);
        Sample amount = numChannelsMinusOne > 0 ? channel / numChannelsMinusOne : 0.0;
        for (size_t sample = 0; sample < getNumSamples(); ++sample) {
            Sample &input = inputChannel[sample];
            Sample &spread = spreadChannel[sample];
            Sample &mode = modeChannel[sample];
            Sample &output = outputChannel[sample];
            output = input + spread * (numChannelsMinusOne > 0 ? amount - 0.5 * mode : amount);
        }
    }
}
