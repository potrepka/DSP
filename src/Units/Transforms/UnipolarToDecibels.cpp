#include "UnipolarToDecibels.h"

dsp::UnipolarToDecibels::UnipolarToDecibels() : Processor(Connection::Type::UNIPOLAR, Connection::Type::DECIBELS) {}

void dsp::UnipolarToDecibels::process() {
    Unit::process();
    for (std::size_t i = 0; i < getNumChannels(); i++) {
        std::transform(getInputSignal()->getChannel(i)->getBuffer().begin(),
                       getInputSignal()->getChannel(i)->getBuffer().end(),
                       getOutputSignal()->getChannel(i)->getBuffer().begin(),
                       amplitudeToDecibels);
    }
}
