#include "MidSide.h"

dsp::MidSide::MidSide(Type type, Space space)
        : Consumer(type, space)
        , mixAmount(std::make_shared<Input>(Type::RATIO, space, 1.0, 1.0))
        , mid(std::make_shared<Output>(type, space))
        , side(std::make_shared<Output>(type, space))
        , data(0, 0)
        , wrapper(data) {
    getInputs().push_back(mixAmount);
    getOutputs().push_back(mid);
    getOutputs().push_back(side);
}

std::shared_ptr<dsp::Input> dsp::MidSide::getMixAmount() const {
    return mixAmount;
}

std::shared_ptr<dsp::Output> dsp::MidSide::getMid() const {
    return mid;
}

std::shared_ptr<dsp::Output> dsp::MidSide::getSide() const {
    return side;
}

void dsp::MidSide::setNumSamplesNoLock(size_t numSamples) {
    Node::setNumSamplesNoLock(numSamples);
    data.setSize(1, numSamples);
    wrapper = Wrapper(data);
}

void dsp::MidSide::processNoLock() {
    if (getNumChannels() > 0) {
        wrapper.clear();
        std::vector<Wrapper> wrappers(getNumChannels());
        for (size_t channel = 0; channel < getNumChannels(); ++channel) {
            wrapper.add(getInput()->getWrapper().getSingleChannel(channel));
        }
        for (size_t channel = 0; channel < getNumChannels(); ++channel) {
            Sample *inputChannel = getInput()->getWrapper().getChannelPointer(channel);
            Sample *mixAmountChannel = getMixAmount()->getWrapper().getChannelPointer(channel);
            Sample *midChannel = getMid()->getWrapper().getChannelPointer(channel);
            Sample *sideChannel = getSide()->getWrapper().getChannelPointer(channel);
            Sample *wrapperChannel = wrapper.getChannelPointer(0);
            for (size_t sample = 0; sample < getNumSamples(); ++sample) {
                Sample &input = inputChannel[sample];
                Sample &mixAmount = mixAmountChannel[sample];
                Sample &mid = midChannel[sample];
                Sample &side = sideChannel[sample];
                Sample &mixed = wrapperChannel[sample];
                mid = input + mixAmount * (mixed - input);
                side = mixAmount * (input - mixed);
            }
        }
    }
}
