#include "PowerDistortion.h"

const std::size_t dsp::PowerDistortion::DRIVE = 1;

dsp::PowerDistortion::PowerDistortion() : Processor(Connection::Type::BIPOLAR, Connection::Type::BIPOLAR) {
    pushInput(Connection::Type::RATIO);
}

std::shared_ptr<dsp::Unit::InputParameter> dsp::PowerDistortion::getDrive() {
    return getInput(DRIVE);
}

void dsp::PowerDistortion::process() {
    Unit::process();
    for (std::size_t i = 0; i < getNumChannels(); i++) {
        std::transform(getInputSignal()->getChannel(i)->getBuffer().begin(),
                       getInputSignal()->getChannel(i)->getBuffer().end(),
                       getDrive()->getChannel(i)->getBuffer().begin(),
                       getOutputSignal()->getChannel(i)->getBuffer().begin(),
                       [](DSP_FLOAT x, DSP_FLOAT y) {
                           x = clip(x, -1.0, 1.0);
                           return x < 0.0f ? powf(1.0f + x, y) - 1.0f : 1.0f - powf(1.0f - x, y);
                       });
    }
}
