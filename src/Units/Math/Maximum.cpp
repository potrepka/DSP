#include "Maximum.h"

dsp::Maximum::Maximum(Type type, Space space)
        : Generator(type, space) {}

unsigned int dsp::Maximum::pushInput() {
    return Unit::pushInput(
            getOutputSignal()->getType(), getOutputSignal()->getSpace(), -std::numeric_limits<DSP_FLOAT>::infinity());
}

void dsp::Maximum::process() {
    Unit::process();
    for (unsigned int i = 0; i < getNumChannels(); i++) {
        if (getNumInputs() > 0) {
            getOutputSignal()->getChannel(i)->fillBuffer(-std::numeric_limits<DSP_FLOAT>::infinity());
            for (const auto &input : inputs) {
                std::transform(getOutputSignal()->getChannel(i)->getBuffer().begin(),
                               getOutputSignal()->getChannel(i)->getBuffer().end(),
                               input->getChannel(i)->getBuffer().begin(),
                               getOutputSignal()->getChannel(i)->getBuffer().begin(),
                               [](DSP_FLOAT x, DSP_FLOAT y) { return fmax(x, y); });
            }
        }
    }
}
