#include "SecondsToHertz.h"

dsp::SecondsToHertz::SecondsToHertz() : Processor(Connection::Type::SECONDS, Connection::Type::HERTZ) {}

void dsp::SecondsToHertz::process() {
    Processor::process();
    for (std::size_t i = 0; i < getNumChannels(); i++) {
        std::transform(getInputSignal()->getChannel(i)->getBuffer().begin(),
                       getInputSignal()->getChannel(i)->getBuffer().end(),
                       getOutputSignal()->getChannel(i)->getBuffer().begin(),
                       [](DSP_FLOAT x) { return 1.0 / x; });
    }
}
