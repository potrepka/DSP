#include "MultiplyHertzSeconds.h"

dsp::MultiplyHertzSeconds::MultiplyHertzSeconds(Space space)
        : Generator(Type::RATIO, space)
        , frequency(pushInput(Type::HERTZ, space))
        , time(pushInput(Type::SECONDS, space)) {}

std::shared_ptr<dsp::InputParameter> dsp::MultiplyHertzSeconds::getFrequency() const {
    return frequency;
}

std::shared_ptr<dsp::InputParameter> dsp::MultiplyHertzSeconds::getTime() const {
    return time;
}

void dsp::MultiplyHertzSeconds::process() {
    Unit::process();
    for (unsigned int i = 0; i < getNumChannels(); ++i) {
        Array &frequencyBuffer = getFrequency()->getChannel(i)->getBuffer();
        Array &timeBuffer = getTime()->getChannel(i)->getBuffer();
        Array &outputBuffer = getOutputSignal()->getChannel(i)->getBuffer();
        Iterator frequencyIterator = frequencyBuffer.begin();
        Iterator timeIterator = timeBuffer.begin();
        Iterator outputIterator = outputBuffer.begin();
        while (outputIterator != outputBuffer.end()) {
#if DSP_USE_VC
            Vector f = *frequencyIterator;
            Vector t = *timeIterator;
#else
            Sample f = *frequencyIterator;
            Sample t = *timeIterator;
#endif
            *outputIterator = f * t;
            ++frequencyIterator;
            ++timeIterator;
            ++outputIterator;
        }
    }
}
