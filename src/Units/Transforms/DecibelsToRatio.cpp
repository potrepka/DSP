#include "DecibelsToRatio.h"

dsp::DecibelsToRatio::DecibelsToRatio() : Processor(Connection::Type::DECIBELS, Connection::Type::RATIO) {}

void dsp::DecibelsToRatio::process() {
    Unit::process();
    for (std::size_t i = 0; i < getNumChannels(); i++) {
        std::transform(getInputSignal()->getChannel(i)->getBuffer().begin(),
                       getInputSignal()->getChannel(i)->getBuffer().end(),
                       getOutputSignal()->getChannel(i)->getBuffer().begin(),
                       decibelsToAmplitude);
    }
}
