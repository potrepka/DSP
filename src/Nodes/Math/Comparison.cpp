#include "Comparison.h"

dsp::Comparison::Comparison(Type type, Space space)
        : Transformer(type, Type::INTEGER, space)
        , threshold(std::make_shared<Input>(type, space)) {
    getInputs().push_back(threshold);
}

std::shared_ptr<dsp::Input> dsp::Comparison::getThreshold() const {
    return threshold;
}

void dsp::Comparison::processNoLock() {
    getOutput()->getWrapper().replaceWithApplicationOf(
            [this](Sample x, Sample y) {
                const Sample diff = x - y;
                return (0.0 < diff) - (diff < 0.0);
            },
            getInput()->getWrapper(),
            getThreshold()->getWrapper());
}
