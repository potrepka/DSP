#include "ToDecibels.h"

dsp::ToDecibels::ToDecibels() : Filter(Connection::Type::UNIPOLAR) {
    getOutputSignal()->setType(Connection::Type::DECIBELS);
}

void dsp::ToDecibels::process() {
    Filter::process();
    for (std::size_t i = 0; i < getNumChannels(); i++) {
        std::transform(getInputSignal()->getChannel(i)->getBuffer().begin(),
                       getInputSignal()->getChannel(i)->getBuffer().end(),
                       getOutputSignal()->getChannel(i)->getBuffer().begin(),
                       toDecibels);
    }
}
