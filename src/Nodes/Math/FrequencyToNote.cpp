#include "FrequencyToNote.h"

dsp::FrequencyToNote::FrequencyToNote(Space space)
        : Transformer(Type::HERTZ, Type::LOGARITHMIC, space)
        , tuningFrequency(std::make_shared<Input>(Type::HERTZ, space, 440.0)) {
    getInputs().push_back(tuningFrequency);
}

std::shared_ptr<dsp::Input> dsp::FrequencyToNote::getTuningFrequency() const {
    return tuningFrequency;
}

void dsp::FrequencyToNote::processNoLock() {
    transform(getTuningFrequency(), [](Sample x, Sample y) { return y == 0.0 ? 0.0 : 12.0 * log2(x / y) + 69.0; });
}
