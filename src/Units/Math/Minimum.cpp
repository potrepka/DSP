#include "Minimum.h"

dsp::Minimum::Minimum(Type type, Space space)
        : Generator(type, space) {}

std::shared_ptr<dsp::InputParameter> dsp::Minimum::pushInput() {
    return Unit::pushInput(
            getOutputSignal()->getType(), getOutputSignal()->getSpace(), std::numeric_limits<Sample>::infinity());
}

void dsp::Minimum::process() {
    Unit::process();
    for (unsigned int i = 0; i < getNumChannels(); i++) {
        if (getNumInputs() > 0) {
            getOutputSignal()->getChannel(i)->fillBuffer(std::numeric_limits<Sample>::infinity());
            for (const auto &input : inputs) {
                std::transform(getOutputSignal()->getChannel(i)->getBuffer().begin(),
                               getOutputSignal()->getChannel(i)->getBuffer().end(),
                               input->getChannel(i)->getBuffer().begin(),
                               getOutputSignal()->getChannel(i)->getBuffer().begin(),
                               [](Sample x, Sample y) { return fmin(x, y); });
            }
        }
    }
}
