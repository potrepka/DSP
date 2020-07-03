#include "RatioToDecibels.h"

dsp::RatioToDecibels::RatioToDecibels() : Processor(Connection::Type::RATIO, Connection::Type::DECIBELS) {}

void dsp::RatioToDecibels::process() {
    Unit::process();
    for (std::size_t i = 0; i < getNumChannels(); i++) {
        std::transform(getInputSignal()->getChannel(i)->getBuffer().begin(),
                       getInputSignal()->getChannel(i)->getBuffer().end(),
                       getOutputSignal()->getChannel(i)->getBuffer().begin(),
                       amplitudeToDecibels);
    }
}
