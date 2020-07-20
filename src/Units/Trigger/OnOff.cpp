#include "OnOff.h"

dsp::OnOff::OnOff()
        : Generator(Type::BINARY)
        , onTrigger(pushInput(Type::BINARY))
        , offTrigger(pushInput(Type::BINARY)) {}

std::shared_ptr<dsp::InputParameter> dsp::OnOff::getOnTrigger() const {
    return onTrigger;
}

std::shared_ptr<dsp::InputParameter> dsp::OnOff::getOffTrigger() const {
    return offTrigger;
}

void dsp::OnOff::setNumChannelsNoLock(unsigned int numChannels) {
    Unit::setNumChannelsNoLock(numChannels);
    state.resize(numChannels, std::numeric_limits<Sample>::quiet_NaN());
}

void dsp::OnOff::process() {
    Unit::process();
    for (unsigned int i = 0; i < getNumChannels(); ++i) {
        Array &onTriggerBuffer = getOnTrigger()->getChannel(i)->getBuffer();
        Array &offTriggerBuffer = getOffTrigger()->getChannel(i)->getBuffer();
        Array &outputBuffer = getOutputSignal()->getChannel(i)->getBuffer();
        Iterator onTriggerIterator = onTriggerBuffer.begin();
        Iterator offTriggerIterator = offTriggerBuffer.begin();
        Iterator outputIterator = outputBuffer.begin();
        while (outputIterator != outputBuffer.end()) {
#if DSP_USE_VC
            Vector stateVector;
            for (int k = 0; k < Vector::Size; ++k) {
                if ((*onTriggerIterator)[k]) {
                    state[i] = 1;
                }
                if ((*offTriggerIterator)[k]) {
                    state[i] = 0;
                }
                stateVector[k] = state[i];
            }
            *outputIterator =
                    Vc::iif(Vc::isnan(stateVector), Vector(getOutputSignal()->getDefaultValue()), stateVector);
#else
            if (*onTriggerIterator) {
                state[i] = 1;
            }
            if (*offTriggerIterator) {
                state[i] = 0;
            }
            *outputIterator = std::isnan(state[i]) ? getOutputSignal()->getDefaultValue() : state[i];
#endif
            ++onTriggerIterator;
            ++offTriggerIterator;
            ++outputIterator;
        }
    }
}
