#include "OneOver.h"

dsp::OneOver::OneOver() : Processor(Connection::Type::RATIO, Connection::Type::RATIO) {}

void dsp::OneOver::process() {
    Unit::process();
    for (std::size_t i = 0; i < getNumChannels(); i++) {
        std::transform(getInputSignal()->getChannel(i)->getBuffer().begin(),
                       getInputSignal()->getChannel(i)->getBuffer().end(),
                       getOutputSignal()->getChannel(i)->getBuffer().begin(),
                       [](DSP_FLOAT x) { return 1.0 / x; });
    }
}
