#include "Consumer.h"

const std::size_t dsp::Consumer::INPUT_SIGNAL = 0;

dsp::Consumer::Consumer(Connection::Type type) {
    pushInput(type);
}

std::size_t dsp::Consumer::getNumChannels() {
    return getInputSignal()->getNumChannels();
}

std::shared_ptr<dsp::Unit::InputParameter> dsp::Consumer::getInputSignal() {
    return getInput(INPUT_SIGNAL);
}

void dsp::Consumer::process() {
    for (const auto& channel : getInputSignal()->getChannels()) {
        channel->copyBuffers();
    }
}
