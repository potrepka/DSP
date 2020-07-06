#include "BipolarToUnipolar.h"

dsp::BipolarToUnipolar::BipolarToUnipolar() : Processor(Connection::Type::BIPOLAR, Connection::Type::UNIPOLAR) {}

void dsp::BipolarToUnipolar::process() {
    Unit::process();
    for (unsigned int i = 0; i < getNumChannels(); i++) {
        std::transform(getInputSignal()->getChannel(i)->getBuffer().begin(),
                       getInputSignal()->getChannel(i)->getBuffer().end(),
                       getOutputSignal()->getChannel(i)->getBuffer().begin(),
                       bipolarToUnipolar);
    }
}
