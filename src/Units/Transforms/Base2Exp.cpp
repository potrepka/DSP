#include "Base2Exp.h"

dsp::Base2Exp::Base2Exp() : Processor(Connection::Type::LINEAR, Connection::Type::RATIO) {}

void dsp::Base2Exp::process() {
    Unit::process();
    for (std::size_t i = 0; i < getNumChannels(); i++) {
        std::transform(getInputSignal()->getChannel(i)->getBuffer().begin(),
                       getInputSignal()->getChannel(i)->getBuffer().end(),
                       getOutputSignal()->getChannel(i)->getBuffer().begin(),
                       [](DSP_FLOAT x) { return exp2(x); });
    }
}
