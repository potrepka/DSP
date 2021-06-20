#include "NoteToFrequency.h"

dsp::NoteToFrequency::NoteToFrequency(Space space)
        : Transformer(Type::RATIO, Type::HERTZ, space)
        , tuningFrequency(std::make_shared<Input>(Type::HERTZ, space)) {
    getInput()->setDefaultValue(69.0);
    getInputs().push_back(tuningFrequency);
}

std::shared_ptr<dsp::Input> dsp::NoteToFrequency::getTuningFrequency() const {
    return tuningFrequency;
}

void dsp::NoteToFrequency::processNoLock() {
    getOutput()->getWrapper().replaceWithSumOf(getInput()->getWrapper(), -69.0);
    getOutput()->getWrapper().multiplyBy(0.08333333333333333);
    getOutput()->getWrapper().apply([](Sample x) { return exp2(x); });
    getOutput()->getWrapper().multiplyBy(getTuningFrequency()->getWrapper());
}
