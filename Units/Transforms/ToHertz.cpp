#include "ToHertz.h"

dsp::ToHertz::ToHertz() : Filter(Connection::Type::SECONDS) {
    getOutputSignal()->setType(Connection::Type::HERTZ);
}

void dsp::ToHertz::process() {
    Filter::process();
    for (std::size_t i = 0; i < getNumChannels(); i++) {
        std::transform(getInputSignal()->getChannel(i)->getBuffer().begin(),
                       getInputSignal()->getChannel(i)->getBuffer().end(),
                       getOutputSignal()->getChannel(i)->getBuffer().begin(),
                       toHertz);
    }
}
