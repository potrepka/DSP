#include "Maximum.h"

dsp::Maximum::Maximum(Type type, Space space) : Processor(type, type, space), type(type), space(space) {}

void dsp::Maximum::pushInput() {
    Unit::pushInput(type, space);
}

void dsp::Maximum::process() {
    Unit::process();
    for (unsigned int i = 0; i < getNumChannels(); i++) {
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
