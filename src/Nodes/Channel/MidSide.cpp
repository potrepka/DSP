#include "MidSide.h"

dsp::MidSide::MidSide(Type type, Domain domain)
    : Consumer(type, domain),
      mixAmount(std::make_shared<Input>(Type::RATIO, domain, 1.0, 1.0)),
      mid(std::make_shared<Output>(type, domain)),
      side(std::make_shared<Output>(type, domain)),
      mixedData(0, 0),
      mixed(mixedData) {
  getInputs().push_back(mixAmount);
  getOutputs().push_back(mid);
  getOutputs().push_back(side);
}

std::shared_ptr<dsp::Input> dsp::MidSide::getMixAmount() const {
  return mixAmount;
}

std::shared_ptr<dsp::Output> dsp::MidSide::getMid() const { return mid; }

std::shared_ptr<dsp::Output> dsp::MidSide::getSide() const { return side; }

void dsp::MidSide::setNumSamplesNoLock(size_t numSamples) {
  Node::setNumSamplesNoLock(numSamples);
  mixedData.setSize(1, numSamples);
  mixed = Wrapper(mixedData);
}

void dsp::MidSide::processNoLock() {
  if (getNumChannels() > 0) {
    mixed.clear();
    std::vector<Wrapper> wrappers(getNumChannels());
    for (size_t channel = 0; channel < getNumChannels(); ++channel) {
      mixed.add(getInput()->getWrapper().getSingleChannel(channel));
    }
    for (size_t channel = 0; channel < getNumChannels(); ++channel) {
      Sample* inputChannel =
          getInput()->getWrapper().getChannelPointer(channel);
      Sample* mixedChannel = mixed.getChannelPointer(0);
      Sample* mixAmountChannel =
          getMixAmount()->getWrapper().getChannelPointer(channel);
      Sample* midChannel = getMid()->getWrapper().getChannelPointer(channel);
      Sample* sideChannel = getSide()->getWrapper().getChannelPointer(channel);
      for (size_t sample = 0; sample < getNumSamples(); ++sample) {
        Sample input = inputChannel[sample];
        Sample mixed = mixedChannel[sample];
        Sample mixAmount = mixAmountChannel[sample];
        Sample& mid = midChannel[sample];
        Sample& side = sideChannel[sample];
        mid = input + mixAmount * (mixed - input);
        side = mixAmount * (input - mixed);
      }
    }
  }
}
