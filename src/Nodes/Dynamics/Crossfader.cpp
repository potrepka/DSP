#include "Crossfader.h"

dsp::Crossfader::Crossfader(Type type, Space space)
        : Consumer(type, space)
        , position(std::make_shared<Input>(Type::RATIO, space))
        , a(std::make_shared<Output>(type, space))
        , b(std::make_shared<Output>(type, space)) {
    getInputs().push_back(position);
    getOutputs().push_back(a);
    getOutputs().push_back(b);
}

std::shared_ptr<dsp::Input> dsp::Crossfader::getPosition() const {
    return position;
}

std::shared_ptr<dsp::Output> dsp::Crossfader::getA() const {
    return a;
}

std::shared_ptr<dsp::Output> dsp::Crossfader::getB() const {
    return b;
}

void dsp::Crossfader::processNoLock() {
    for (size_t channel = 0; channel < getNumChannels(); ++channel) {
        Sample *inputChannel = getInput()->getWrapper().getChannelPointer(channel);
        Sample *positionChannel = getPosition()->getWrapper().getChannelPointer(channel);
        Sample *aChannel = getA()->getWrapper().getChannelPointer(channel);
        Sample *bChannel = getB()->getWrapper().getChannelPointer(channel);
        for (size_t sample = 0; sample < getNumSamples(); ++sample) {
            Sample &input = inputChannel[sample];
            Sample &position = positionChannel[sample];
            Sample &a = aChannel[sample];
            Sample &b = bChannel[sample];
            a = (1.0 - position) * input;
            b = position * input;
        }
    }
}
