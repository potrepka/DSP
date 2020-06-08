#include "ToSeconds.h"

dsp::ToSeconds::ToSeconds() : Processor(Connection::Type::HERTZ, Connection::Type::SECONDS) {}

void dsp::ToSeconds::process() {
    Processor::process();
    for (std::size_t i = 0; i < getNumChannels(); i++) {
        std::transform(getInputSignal()->getChannel(i)->getBuffer().begin(),
                       getInputSignal()->getChannel(i)->getBuffer().end(),
                       getOutputSignal()->getChannel(i)->getBuffer().begin(),
                       toSeconds);
    }
}
