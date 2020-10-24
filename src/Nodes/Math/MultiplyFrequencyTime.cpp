#include "MultiplyFrequencyTime.h"

dsp::MultiplyFrequencyTime::MultiplyFrequencyTime(Space space)
        : Producer(Type::RATIO, space)
        , frequency(std::make_shared<Input>(Type::HERTZ, space))
        , time(std::make_shared<Input>(Type::SECONDS, space)) {
    getInputs().push_back(frequency);
    getInputs().push_back(time);
}

std::shared_ptr<dsp::Input> dsp::MultiplyFrequencyTime::getFrequency() const {
    return frequency;
}

std::shared_ptr<dsp::Input> dsp::MultiplyFrequencyTime::getTime() const {
    return time;
}

void dsp::MultiplyFrequencyTime::processNoLock() {
    getOutput()->getWrapper().replaceWithProductOf(getFrequency()->getWrapper(), getTime()->getWrapper());
}
