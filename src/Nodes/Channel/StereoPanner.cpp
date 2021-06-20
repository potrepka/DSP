#include "StereoPanner.h"

dsp::StereoPanner::StereoPanner(Type type, Space space)
        : Consumer(type, space)
        , direction(std::make_shared<Input>(Type::RATIO, space, 1.0, 0.5))
        , left(std::make_shared<Output>(type, space))
        , right(std::make_shared<Output>(type, space)) {
    getInputs().push_back(direction);
    getOutputs().push_back(left);
    getOutputs().push_back(right);
}

std::shared_ptr<dsp::Input> dsp::StereoPanner::getDirection() const {
    return direction;
}

std::shared_ptr<dsp::Output> dsp::StereoPanner::getLeft() const {
    return left;
}

std::shared_ptr<dsp::Output> dsp::StereoPanner::getRight() const {
    return right;
}

void dsp::StereoPanner::processNoLock() {
    for (size_t channel = 0; channel < getNumChannels(); ++channel) {
        Sample *inputChannel = getInput()->getWrapper().getChannelPointer(channel);
        Sample *directionChannel = getDirection()->getWrapper().getChannelPointer(channel);
        Sample *leftChannel = getLeft()->getWrapper().getChannelPointer(channel);
        Sample *rightChannel = getRight()->getWrapper().getChannelPointer(channel);
        for (size_t sample = 0; sample < getNumSamples(); ++sample) {
            Sample &direction = directionChannel[sample];
            leftChannel[sample] = SQRT_OF_TWO * cos(PI_OVER_TWO * direction) * inputChannel[sample];
            rightChannel[sample] = SQRT_OF_TWO * cos(PI_OVER_TWO * (1.0 - direction)) * inputChannel[sample];
        }
    }
}
