#include "FunctionOscillator.h"

dsp::FunctionOscillator::FunctionOscillator(Type type)
        : Producer(type)
        , phase(std::make_shared<Input>(Type::RATIO)) {
    getInputs().push_back(phase);
}

std::function<dsp::Sample(dsp::Sample)> dsp::FunctionOscillator::getFunction() {
    return function;
}

void dsp::FunctionOscillator::setFunction(std::function<Sample(Sample)> function) {
    this->function = function;
}

std::shared_ptr<dsp::Input> dsp::FunctionOscillator::getPhase() {
    return phase;
}

void dsp::FunctionOscillator::processNoLock() {
    if (function) {
        for (int channel = 0; channel < getNumChannels(); ++channel) {
            Sample *phaseChannel = getPhase()->getWrapper().getChannelPointer(channel);
            Sample *outputChannel = getOutput()->getWrapper().getChannelPointer(channel);
            for (int sample = 0; sample < getNumSamples(); ++sample) {
                outputChannel[sample] = function(phaseChannel[sample]);
            }
        }
    }
}
