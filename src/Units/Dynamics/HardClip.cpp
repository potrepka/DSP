#include "HardClip.h"

dsp::HardClip::HardClip() : Processor(Connection::Type::BIPOLAR, Connection::Type::BIPOLAR) {}

void dsp::HardClip::process() {
    Unit::process();
    for (unsigned int i = 0; i < getNumChannels(); i++) {
        std::transform(getInputSignal()->getChannel(i)->getBuffer().begin(),
                       getInputSignal()->getChannel(i)->getBuffer().end(),
                       getOutputSignal()->getChannel(i)->getBuffer().begin(),
                       [](DSP_FLOAT x) { return clip(x, -1.0, 1.0); });
    }
}
