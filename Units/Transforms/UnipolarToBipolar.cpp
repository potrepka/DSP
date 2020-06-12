#include "UnipolarToBipolar.h"

dsp::UnipolarToBipolar::UnipolarToBipolar() : Processor(Connection::Type::UNIPOLAR, Connection::Type::BIPOLAR) {}

void dsp::UnipolarToBipolar::process() {
    Unit::process();
    for (std::size_t i = 0; i < getNumChannels(); i++) {
        std::transform(getInputSignal()->getChannel(i)->getBuffer().begin(),
                       getInputSignal()->getChannel(i)->getBuffer().end(),
                       getOutputSignal()->getChannel(i)->getBuffer().begin(),
                       [](DSP_FLOAT x) { return 2.0 * x - 1.0; });
    }
}
