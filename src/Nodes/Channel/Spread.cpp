#include "Spread.h"

dsp::Spread::Spread(Type type, Space space)
        : Transformer(type, type, space)
        , mode(Mode::UNIPOLAR)
        , spread(std::make_shared<Input>(type, space)) {}

dsp::Spread::Mode dsp::Spread::getMode() const {
    return mode;
}

void dsp::Spread::setMode(Mode mode) {
    this->mode = mode;
}

std::shared_ptr<dsp::Input> dsp::Spread::getSpread() const {
    return spread;
}

void dsp::Spread::processNoLock() {
    size_t numChannelsMinusOne = getNumChannels() - 1;
    Sample bias;
    switch (mode) {
        case Mode::UNIPOLAR: bias = 0.0; break;
        case Mode::BIPOLAR: bias = -0.5; break;
    }
    for (size_t channel = 0; channel < getNumChannels(); ++channel) {
        Sample amount = numChannelsMinusOne > 0 ? channel / numChannelsMinusOne + bias : 0.0;
        getOutput()->getWrapper().getSingleChannel(channel).copyFrom(
                getInput()->getWrapper().getSingleChannel(channel));
        getOutput()->getWrapper().getSingleChannel(channel).addProductOf(
                getSpread()->getWrapper().getSingleChannel(channel), amount);
    }
}
