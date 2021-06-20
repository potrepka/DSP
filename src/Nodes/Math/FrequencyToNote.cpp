#include "FrequencyToNote.h"

dsp::FrequencyToNote::FrequencyToNote(Space space)
        : Transformer(Type::HERTZ, Type::RATIO, space)
        , tuningFrequency(std::make_shared<Input>(Type::HERTZ, space)) {
    getInputs().push_back(tuningFrequency);
}

std::shared_ptr<dsp::Input> dsp::FrequencyToNote::getTuningFrequency() const {
    return tuningFrequency;
}

void dsp::FrequencyToNote::processNoLock() {
    getOutput()->getWrapper().replaceWithApplicationOf([](Sample x, Sample y) { return y == 0.0 ? 0.0 : log2(x / y); },
                                                       getInput()->getWrapper(),
                                                       getTuningFrequency()->getWrapper());
    getOutput()->getWrapper().multiplyBy(12.0);
    getOutput()->getWrapper().add(69.0);
}
