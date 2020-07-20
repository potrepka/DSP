#include "OrGate.h"

dsp::OrGate::OrGate(Space space)
        : Generator(Type::BINARY, space) {}

std::shared_ptr<dsp::InputParameter> dsp::OrGate::pushInput() {
    return Unit::pushInput(Type::BINARY, getOutputSignal()->getSpace(), 0.0);
}

void dsp::OrGate::process() {
    Unit::process();
    if (getNumInputs() > 0) {
        for (unsigned int i = 0; i < getNumChannels(); ++i) {
            getOutputSignal()->getChannel(i)->fillBuffer(0.0);
        }
    }
    for (const auto &input : inputs) {
        transformOutput(input,
#if DSP_USE_VC
                        [](Vector x, Vector y) { return Vc::iif(x || y, Vector::One(), Vector::Zero()); });
#else
                        [](Sample x, Sample y) { return x || y; });
#endif
    }
}
