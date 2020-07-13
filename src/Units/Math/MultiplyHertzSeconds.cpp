#include "MultiplyHertzSeconds.h"

const unsigned int dsp::MultiplyHertzSeconds::FREQUENCY = 0;
const unsigned int dsp::MultiplyHertzSeconds::TIME = 1;

dsp::MultiplyHertzSeconds::MultiplyHertzSeconds(Space space) : Generator(Type::RATIO, space) {
    pushInput(Type::HERTZ, space);
    pushInput(Type::SECONDS, space);
}

std::shared_ptr<dsp::Unit::InputParameter> dsp::MultiplyHertzSeconds::getFrequency() const {
    return getInput(FREQUENCY);
}

std::shared_ptr<dsp::Unit::InputParameter> dsp::MultiplyHertzSeconds::getTime() const {
    return getInput(TIME);
}

void dsp::MultiplyHertzSeconds::process() {
    Unit::process();
    for (unsigned int i = 0; i < getNumChannels(); i++) {
        std::transform(getFrequency()->getChannel(i)->getBuffer().begin(),
                       getFrequency()->getChannel(i)->getBuffer().end(),
                       getTime()->getChannel(i)->getBuffer().begin(),
                       getOutputSignal()->getChannel(i)->getBuffer().begin(),
                       std::multiplies<DSP_FLOAT>());
    }
}
