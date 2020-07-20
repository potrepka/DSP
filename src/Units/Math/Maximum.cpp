#include "Maximum.h"

dsp::Maximum::Maximum(Type type, Space space)
        : Generator(type, space) {}

std::shared_ptr<dsp::InputParameter> dsp::Maximum::pushInput() {
    return Unit::pushInput(
            getOutputSignal()->getType(), getOutputSignal()->getSpace(), -std::numeric_limits<Sample>::infinity());
}

void dsp::Maximum::process() {
    Unit::process();
    if (getNumInputs() > 0) {
        for (unsigned int i = 0; i < getNumChannels(); ++i) {
            getOutputSignal()->getChannel(i)->fillBuffer(-std::numeric_limits<Sample>::infinity());
        }
    }
    for (const auto &input : inputs) {
        transformOutput(input,
#if DSP_USE_VC
                        [](Vector x, Vector y) { return Vc::max(x, y); });
#else
                        [](Sample x, Sample y) { return fmax(x, y); });
#endif
    }
}
