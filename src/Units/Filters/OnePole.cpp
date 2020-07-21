#include "OnePole.h"

dsp::OnePole::OnePole(Type type)
        : Processor(type, type)
        , frequency(pushInput(Type::HERTZ))
        , mode(Mode::LOW_PASS) {}

dsp::OnePole::Mode dsp::OnePole::getMode() const {
    return mode;
}

void dsp::OnePole::setMode(Mode mode) {
    lock();
    this->mode = mode;
    unlock();
}

std::shared_ptr<dsp::InputParameter> dsp::OnePole::getFrequency() const {
    return frequency;
}

void dsp::OnePole::setNumChannelsNoLock(unsigned int numChannels) {
    Unit::setNumChannelsNoLock(numChannels);
    state.resize(numChannels, 0.0);
}

void dsp::OnePole::process() {
    Unit::process();
    for (unsigned int i = 0; i < getNumChannels(); ++i) {
        Array &inputBuffer = getInputSignal()->getChannel(i)->getBuffer();
        Array &frequencyBuffer = getFrequency()->getChannel(i)->getBuffer();
        Array &outputBuffer = getOutputSignal()->getChannel(i)->getBuffer();
        Iterator inputIterator = inputBuffer.begin();
        Iterator frequencyIterator = frequencyBuffer.begin();
        Iterator outputIterator = outputBuffer.begin();
        while (outputIterator != outputBuffer.end()) {
#if DSP_USE_VC
            Vector radians = PI * *frequencyIterator * getOneOverSampleRate();
            Vector gammaSin;
            Vector gammaCos;
            Vc::sincos(radians / (Vector::One() + radians), &gammaSin, &gammaCos);
            Vector gamma = gammaSin / gammaCos;
            Vector outputVector;
            for (int k = 0; k < Vector::Size; ++k) {
                Sample delta = gamma[k] * ((*inputIterator)[k] - state[i]);
                state[i] += delta;
                switch (mode) {
                    case Mode::LOW_PASS: outputVector[k] = state[i]; break;
                    case Mode::HIGH_PASS: outputVector[k] = (*inputIterator)[k] - state[i]; break;
                }
                state[i] += delta;
            }
            *outputIterator = outputVector;
#else
            Sample radians = PI * *frequencyIterator * getOneOverSampleRate();
            Sample delta = tan(radians / (1.0 + radians)) * (*inputIterator - state[i]);
            state[i] += delta;
            switch (mode) {
                case Mode::LOW_PASS: *outputIterator = state[i]; break;
                case Mode::HIGH_PASS: *outputIterator = *inputIterator - state[i]; break;
            }
            state[i] += delta;
#endif

            ++inputIterator;
            ++frequencyIterator;
            ++outputIterator;
        }
    }
}
