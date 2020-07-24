#include "FunctionOscillator.h"

dsp::FunctionOscillator::FunctionOscillator(Type type)
        : Generator(type)
        , phase(pushInput(Type::UNIPOLAR)) {}

std::function<dsp::Sample(dsp::Sample)> dsp::FunctionOscillator::getFunction() {
    return function;
}

void dsp::FunctionOscillator::setFunction(std::function<Sample(Sample)> function) {
    this->function = function;
}

std::shared_ptr<dsp::InputParameter> dsp::FunctionOscillator::getPhase() {
    return phase;
}

void dsp::FunctionOscillator::process() {
    Unit::process();
    for (unsigned int i = 0; i < getNumChannels(); ++i) {
        Array &phaseBuffer = getPhase()->getChannel(i)->getBuffer();
        Array &outputBuffer = getOutputSignal()->getChannel(i)->getBuffer();
        Iterator phaseIterator = phaseBuffer.begin();
        Iterator outputIterator = outputBuffer.begin();
        while (outputIterator != outputBuffer.end()) {
#if DSP_USE_VC
            *outputIterator = (*phaseIterator).apply(function);
#else
            *outputIterator = function(*phaseIterator);
#endif
            ++phaseIterator;
            ++outputIterator;
        }
    }
}
