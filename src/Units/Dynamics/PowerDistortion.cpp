#include "PowerDistortion.h"

const unsigned int dsp::PowerDistortion::DRIVE = 1;

dsp::PowerDistortion::PowerDistortion() : Processor(Connection::Type::BIPOLAR, Connection::Type::BIPOLAR) {
    pushInput(Connection::Type::RATIO);
}

std::shared_ptr<dsp::Unit::InputParameter> dsp::PowerDistortion::getDrive() {
    return getInput(DRIVE);
}

void dsp::PowerDistortion::process() {
    Unit::process();
    for (unsigned int i = 0; i < getNumChannels(); i++) {
        std::transform(getInputSignal()->getChannel(i)->getBuffer().begin(),
                       getInputSignal()->getChannel(i)->getBuffer().end(),
                       getDrive()->getChannel(i)->getBuffer().begin(),
                       getOutputSignal()->getChannel(i)->getBuffer().begin(),
                       [](DSP_FLOAT x, DSP_FLOAT y) {
                           if (y == 0.0) {
                               return 0.0;
                           } else {
                               x = clip(x, -1.0, 1.0);
                               return x < 0.0 ? pow(1.0 + x, y) - 1.0 : 1.0 - pow(1.0 - x, y);
                           }
                       });
    }
}
