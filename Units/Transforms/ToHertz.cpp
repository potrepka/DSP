#include "ToHertz.h"

dsp::ToHertz::ToHertz() : Processor(Connection::Type::SECONDS, Connection::Type::HERTZ) {}

void dsp::ToHertz::process() {
    Processor::process();
    for (std::size_t i = 0; i < getNumChannels(); i++) {
        std::transform(getInputSignal()->getChannel(i)->getBuffer().begin(),
                       getInputSignal()->getChannel(i)->getBuffer().end(),
                       getOutputSignal()->getChannel(i)->getBuffer().begin(),
                       toHertz);
    }
}
