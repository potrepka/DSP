#include "ToSeconds.h"

dsp::ToSeconds::ToSeconds() : Filter(Connection::Type::HERTZ) {
    getOutputSignal()->setType(Connection::Type::SECONDS);
}

void dsp::ToSeconds::process() {
    Filter::process();
    for (std::size_t i = 0; i < getNumChannels(); i++) {
        std::transform(getInputSignal()->getChannel(i)->getBuffer().begin(),
                       getInputSignal()->getChannel(i)->getBuffer().end(),
                       getOutputSignal()->getChannel(i)->getBuffer().begin(),
                       toSeconds);
    }
}
