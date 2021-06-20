#include "ChannelSplitter.h"

dsp::ChannelSplitter::ChannelSplitter(Type type, Space space)
        : Consumer(type, space) {}

std::shared_ptr<dsp::Output> dsp::ChannelSplitter::getOutput(size_t channel) const {
    DSP_ASSERT(channel < outputs.size());
    return outputs[channel];
}

void dsp::ChannelSplitter::setNumInputChannelsNoLock(size_t numChannels) {
    if (numChannels < getNumInputChannels()) {
        outputs.resize(numChannels);
    } else {
        outputs.reserve(numChannels);
        for (size_t channel = getNumInputChannels(); channel < numChannels; ++channel) {
            outputs.push_back(std::make_shared<Output>(getInput()->getType(),
                                                       getInput()->getSpace(),
                                                       getInput()->getRange(),
                                                       getInput()->getDefaultValue(),
                                                       1,
                                                       getInput()->getNumSamples()));
        }
    }
    Node::setNumInputChannelsNoLock(numChannels);
}

void dsp::ChannelSplitter::setNumOutputChannelsNoLock(size_t numChannels) {}

void dsp::ChannelSplitter::processNoLock() {
    for (size_t channel = 0; channel < getNumInputChannels(); ++channel) {
        getOutput(channel)->getWrapper().getSingleChannel(0).copyFrom(
                getInput()->getWrapper().getSingleChannel(channel));
    }
}
