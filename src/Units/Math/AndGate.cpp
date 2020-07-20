#include "AndGate.h"

dsp::AndGate::AndGate(Space space)
        : Generator(Type::BINARY, space) {}

std::shared_ptr<dsp::InputParameter> dsp::AndGate::pushInput() {
    return Unit::pushInput(Type::BINARY, getOutputSignal()->getSpace(), 1.0);
}

void dsp::AndGate::process() {
    Unit::process();
    if (getNumInputs() > 0) {
        for (unsigned int i = 0; i < getNumChannels(); ++i) {
            getOutputSignal()->getChannel(i)->fillBuffer(1.0);
        }
    }
    for (const auto &input : inputs) {
        transformOutput(input,
#if DSP_USE_VC
                        [](Vector x, Vector y) { return Vc::iif(x && y, Vector::One(), Vector::Zero()); });
#else
                        [](Sample x, Sample y) { return x && y; });
#endif
    }
}
