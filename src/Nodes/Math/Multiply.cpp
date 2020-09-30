#include "Multiply.h"

dsp::Multiply::Multiply(Type type, Space space)
        : Transformer(type, type, space) {}

size_t dsp::Multiply::getNumFactors() const {
    return factors.size();
}

void dsp::Multiply::setNumFactors(size_t numFactors) {
    if (numFactors < getNumFactors()) {
        factors.resize(numFactors);
        getInputs().resize(numFactors + 1);
    } else {
        factors.reserve(numFactors);
        getInputs().reserve(numFactors + 1);
        for (size_t i = getNumFactors(); i < numFactors; ++i) {
            std::shared_ptr<dsp::Input> factor = std::make_shared<Input>(Type::RATIO,
                                                                         getInput()->getSpace(),
                                                                         1.0,
                                                                         getInput()->getNumChannels(),
                                                                         getInput()->getNumSamples());
            factors.push_back(factor);
            getInputs().push_back(factor);
        }
    }
}

std::vector<std::shared_ptr<dsp::Input>> &dsp::Multiply::getFactors() {
    return factors;
}

void dsp::Multiply::processNoLock() {
    getOutput()->getWrapper().copyFrom(getInput()->getWrapper());
    for (const auto &factor : factors) {
        getOutput()->getWrapper().replaceWithProductOf(getOutput()->getWrapper(), factor->getWrapper());
    }
}
