#include "StereoPanner.h"

dsp::StereoPanner::StereoPanner(Type type, Domain domain)
    : Consumer(type, domain),
      direction(std::make_shared<Input>(Type::RATIO, domain)),
      left(std::make_shared<Output>(type, domain)),
      right(std::make_shared<Output>(type, domain)) {
  getInputs().push_back(direction);
  getOutputs().push_back(left);
  getOutputs().push_back(right);
}

std::shared_ptr<dsp::Input> dsp::StereoPanner::getDirection() const {
  return direction;
}

std::shared_ptr<dsp::Output> dsp::StereoPanner::getLeft() const { return left; }

std::shared_ptr<dsp::Output> dsp::StereoPanner::getRight() const {
  return right;
}

void dsp::StereoPanner::processNoLock() {
  for (size_t channel = 0; channel < getNumChannels(); ++channel) {
    Sample* inputChannel = getInput()->getWrapper().getChannelPointer(channel);
    Sample* directionChannel =
        getDirection()->getWrapper().getChannelPointer(channel);
    Sample* leftChannel = getLeft()->getWrapper().getChannelPointer(channel);
    Sample* rightChannel = getRight()->getWrapper().getChannelPointer(channel);
    for (size_t sample = 0; sample < getNumSamples(); ++sample) {
      Sample direction = directionChannel[sample];
      Sample amount = 0.5 * (direction + 1.0);
      leftChannel[sample] =
          SQRT_OF_TWO * cos(PI_OVER_TWO * amount) * inputChannel[sample];
      rightChannel[sample] = SQRT_OF_TWO * cos(PI_OVER_TWO * (1.0 - amount)) *
                             inputChannel[sample];
    }
  }
}
