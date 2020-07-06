#include "Base2Log.h"

dsp::Base2Log::Base2Log() : Processor(Connection::Type::RATIO, Connection::Type::LINEAR) {}

void dsp::Base2Log::process() {
    Unit::process();
    for (unsigned int i = 0; i < getNumChannels(); i++) {
        std::transform(getInputSignal()->getChannel(i)->getBuffer().begin(),
                       getInputSignal()->getChannel(i)->getBuffer().end(),
                       getOutputSignal()->getChannel(i)->getBuffer().begin(),
                       [](DSP_FLOAT x) { return log2(x); });
    }
}
