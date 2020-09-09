#include "MidSide.h"

dsp::MidSide::MidSide(Type type, Space space)
        : Consumer(type, space)
        , mixAmount(std::make_shared<Input>(Type::RATIO, space))
        , mid(std::make_shared<Output>(type == Type::INTEGER ? Type::LOGARITHMIC : type, space))
        , side(std::make_shared<Output>(type == Type::INTEGER ? Type::LOGARITHMIC : type, space))
        , data(0, 0)
        , block(data) {
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

void dsp::MidSide::setNumSamplesNoLock(int numSamples) {
    Node::setNumSamplesNoLock(numSamples);
    data.setSize(1, numSamples);
    block = Block(data);
}

void dsp::MidSide::processNoLock() {
    if (getNumChannels() > 0) {
        block.clear();
        std::vector<Block> blocks(getNumChannels());
        for (int channel = 0; channel < getNumChannels(); ++channel) {
            block.add(getInput()->getBlock().getSingleChannelBlock(channel));
        }
        for (int channel = 0; channel < getNumChannels(); ++channel) {
            Sample *inputChannel = getInput()->getBlock().getChannelPointer(channel);
            Sample *mixAmountChannel = getMixAmount()->getBlock().getChannelPointer(channel);
            Sample *midChannel = getMid()->getBlock().getChannelPointer(channel);
            Sample *sideChannel = getSide()->getBlock().getChannelPointer(channel);
            Sample *blockChannel = block.getChannelPointer(0);
            for (int sample = 0; sample < getNumSamples(); ++sample) {
                Sample signal = inputChannel[sample];
                Sample amount = clip(mixAmountChannel[sample], 0.0, 1.0);
                Sample together = blockChannel[sample];
                midChannel[sample] = signal + amount * (together - signal);
                sideChannel[sample] = amount * (signal - together);
            }
        }
    }
}
