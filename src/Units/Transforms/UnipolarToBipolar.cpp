#include "UnipolarToBipolar.h"

dsp::UnipolarToBipolar::UnipolarToBipolar() : Processor(Connection::Type::UNIPOLAR, Connection::Type::BIPOLAR) {}

void dsp::UnipolarToBipolar::process() {
    Unit::process();
    for (unsigned int i = 0; i < getNumChannels(); i++) {
        std::transform(getInputSignal()->getChannel(i)->getBuffer().begin(),
                       getInputSignal()->getChannel(i)->getBuffer().end(),
                       getOutputSignal()->getChannel(i)->getBuffer().begin(),
                       unipolarToBipolar);
    }
}
