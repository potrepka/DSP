#include "StereoPanner.h"

dsp::StereoPanner::StereoPanner(Type type, Space space)
        : Consumer(type, space)
        , direction(std::make_shared<Input>(Type::RATIO, space))
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
    for (int channel = 0; channel < getNumChannels(); ++channel) {
        Sample *inputChannel = getInput()->getWrapper().getChannelPointer(channel);
        Sample *directionChannel = getDirection()->getWrapper().getChannelPointer(channel);
        Sample *leftChannel = getLeft()->getWrapper().getChannelPointer(channel);
        Sample *rightChannel = getRight()->getWrapper().getChannelPointer(channel);
        for (int sample = 0; sample < getNumSamples(); ++sample) {
            Sample direction = clip(directionChannel[sample], -1.0, 1.0);
            leftChannel[sample] = SQRT2 * cos(PI_OVER_TWO * (0.5 * direction + 0.5)) * inputChannel[sample];
            rightChannel[sample] = SQRT2 * cos(PI_OVER_TWO * (-0.5 * direction - 0.5)) * inputChannel[sample];
        }
    }
}
