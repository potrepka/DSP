#include "MultiplyHertzSeconds.h"

dsp::MultiplyHertzSeconds::MultiplyHertzSeconds(Space space)
        : Producer(Type::RATIO, space)
        , frequency(std::make_shared<Input>(Type::HERTZ, space))
        , time(std::make_shared<Input>(Type::SECONDS, space)) {
    getInputs().push_back(frequency);
    getInputs().push_back(time);
}

std::shared_ptr<dsp::Input> dsp::MultiplyHertzSeconds::getFrequency() const {
    return frequency;
}

std::shared_ptr<dsp::Input> dsp::MultiplyHertzSeconds::getTime() const {
    return time;
}

void dsp::MultiplyHertzSeconds::processNoLock() {
    getOutput()->getBlock().replaceWithProductOf(getFrequency()->getBlock(), getTime()->getBlock());
}
