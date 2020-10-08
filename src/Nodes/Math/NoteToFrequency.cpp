#include "NoteToFrequency.h"

dsp::NoteToFrequency::NoteToFrequency(Space space)
        : Transformer(Type::LOGARITHMIC, Type::HERTZ, space)
        , tuningFrequency(std::make_shared<Input>(Type::HERTZ, space, 440.0)) {
    getInputs().push_back(tuningFrequency);
}

std::shared_ptr<dsp::Input> dsp::NoteToFrequency::getTuningFrequency() const {
    return tuningFrequency;
}

void dsp::NoteToFrequency::processNoLock() {
    transform(getTuningFrequency(), [](Sample x, Sample y) { return exp2((x - 69.0) * 0.08333333333333333) * y; });
}
