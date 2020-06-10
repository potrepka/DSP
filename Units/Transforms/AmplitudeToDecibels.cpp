#include "AmplitudeToDecibels.h"

dsp::AmplitudeToDecibels::AmplitudeToDecibels() : Processor(Connection::Type::UNIPOLAR, Connection::Type::DECIBELS) {}

void dsp::AmplitudeToDecibels::process() {
    Unit::process();
    for (std::size_t i = 0; i < getNumChannels(); i++) {
        std::transform(getInputSignal()->getChannel(i)->getBuffer().begin(),
                       getInputSignal()->getChannel(i)->getBuffer().end(),
                       getOutputSignal()->getChannel(i)->getBuffer().begin(),
                       amplitudeToDecibels);
    }
}
