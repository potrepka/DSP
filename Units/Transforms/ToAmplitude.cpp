#include "ToAmplitude.h"

dsp::ToAmplitude::ToAmplitude() : Filter(Connection::Type::DECIBELS) {
    getOutputSignal()->setType(Connection::Type::UNIPOLAR);
}

void dsp::ToAmplitude::process() {
    Filter::process();
    for (std::size_t i = 0; i < getNumChannels(); i++) {
        std::transform(getInputSignal()->getChannel(i)->getBuffer().begin(),
                       getInputSignal()->getChannel(i)->getBuffer().end(),
                       getOutputSignal()->getChannel(i)->getBuffer().begin(),
                       toAmplitude);
    }
}
