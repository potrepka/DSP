#include "HertzToSeconds.h"

dsp::HertzToSeconds::HertzToSeconds() : Processor(Connection::Type::HERTZ, Connection::Type::SECONDS) {}

void dsp::HertzToSeconds::process() {
    Unit::process();
    for (std::size_t i = 0; i < getNumChannels(); i++) {
        std::transform(getInputSignal()->getChannel(i)->getBuffer().begin(),
                       getInputSignal()->getChannel(i)->getBuffer().end(),
                       getOutputSignal()->getChannel(i)->getBuffer().begin(),
                       [](DSP_FLOAT x) { return 1.0 / x; });
    }
}
