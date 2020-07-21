#include "Biquad.h"

dsp::Biquad::Biquad()
        : Processor(dsp::Type::BIPOLAR, dsp::Type::BIPOLAR)
        , frequency(pushInput(Type::HERTZ))
        , resonance(pushInput(Type::RATIO, Space::TIME, 1.0))
        , gain(pushInput(Type::LINEAR))
        , mode(Mode::LOW_PASS) {}

dsp::Biquad::Mode dsp::Biquad::getMode() const {
    return mode;
}

void dsp::Biquad::setMode(Mode mode) {
    lock();
    this->mode = mode;
    unlock();
}

std::shared_ptr<dsp::InputParameter> dsp::Biquad::getFrequency() const {
    return frequency;
}

std::shared_ptr<dsp::InputParameter> dsp::Biquad::getResonance() const {
    return resonance;
}

std::shared_ptr<dsp::InputParameter> dsp::Biquad::getGain() const {
    return gain;
}

void dsp::Biquad::setNumChannelsNoLock(unsigned int numChannels) {
    Unit::setNumChannelsNoLock(numChannels);
    x1.resize(numChannels, 0.0);
    x2.resize(numChannels, 0.0);
    y1.resize(numChannels, 0.0);
    y2.resize(numChannels, 0.0);
}

void dsp::Biquad::process() {
    Unit::process();
    for (unsigned int i = 0; i < getNumChannels(); ++i) {
        Array &inputBuffer = getInputSignal()->getChannel(i)->getBuffer();
        Array &frequencyBuffer = getFrequency()->getChannel(i)->getBuffer();
        Array &resonanceBuffer = getResonance()->getChannel(i)->getBuffer();
        Array &gainBuffer = getGain()->getChannel(i)->getBuffer();
        Array &outputBuffer = getOutputSignal()->getChannel(i)->getBuffer();
        Iterator inputIterator = inputBuffer.begin();
        Iterator frequencyIterator = frequencyBuffer.begin();
        Iterator resonanceIterator = resonanceBuffer.begin();
        Iterator gainIterator = gainBuffer.begin();
        Iterator outputIterator = outputBuffer.begin();
        while (outputIterator != outputBuffer.end()) {
#if DSP_USE_VC
            calculateCoefficients(*frequencyIterator, *resonanceIterator, *gainIterator);
            Vector outputVector;
            for (int k = 0; k < Vector::Size; ++k) {
                outputVector[k] =
                        (b0[k] * (*inputIterator)[k] + b1[k] * x1[i] + b2[k] * x2[i] - a1[k] * y1[i] - a2[k] * y2[i]) /
                        a0[k];
                x2[i] = x1[i];
                x1[i] = (*inputIterator)[k];
                y2[i] = y1[i];
                y1[i] = outputVector[k];
            }
            *outputIterator = Vc::iif(*resonanceIterator == Vector::Zero(), Vector::Zero(), outputVector);
#else
            if (*resonanceIterator == 0.0) {
                *outputIterator = 0.0;
            } else {
                calculateCoefficients(*frequencyIterator, *resonanceIterator, *gainIterator);
                *outputIterator = (b0 * *inputIterator + b1 * x1[i] + b2 * x2[i] - a1 * y1[i] - a2 * y2[i]) / a0;
            }
            x2[i] = x1[i];
            x1[i] = *inputIterator;
            y2[i] = y1[i];
            y1[i] = *outputIterator;
#endif
            ++inputIterator;
            ++frequencyIterator;
            ++resonanceIterator;
            ++gainIterator;
            ++outputIterator;
        }
    }
}

#if DSP_USE_VC
void dsp::Biquad::calculateCoefficients(const Vector &frequency, const Vector &resonance, const Vector &gain) {
    const Vector omega = TAU * frequency * getOneOverSampleRate();
    const Vector sinW = Vc::sin(omega);
    const Vector cosW = Vc::cos(omega);
    const Vector alpha = sinW / (SQRT2 * resonance);
#else
void dsp::Biquad::calculateCoefficients(const Sample &frequency, const Sample &resonance, const Sample &gain) {
    const Sample omega = TAU * frequency * getOneOverSampleRate();
    const Sample sinW = sin(omega);
    const Sample cosW = cos(omega);
    const Sample alpha = sinW / (SQRT2 * resonance);
#endif

    switch (mode) {
        case Mode::LOW_PASS:
            a0 = 1.0 + alpha;
            a1 = -2.0 * cosW;
            a2 = 1.0 - alpha;
            b1 = 1.0 - cosW;
            b0 = b2 = 0.5 * b1;
            break;
        case Mode::HIGH_PASS:
            a0 = 1.0 + alpha;
            a1 = -2.0 * cosW;
            a2 = 1.0 - alpha;
            b1 = -1.0 - cosW;
            b0 = b2 = -0.5 * b1;
            break;
        case Mode::BAND_PASS:
            a0 = 1.0 + alpha;
            a1 = -2.0 * cosW;
            a2 = 1.0 - alpha;
            b0 = alpha;
            b1 = 0.0;
            b2 = -alpha;
            break;
        case Mode::BAND_STOP:
            a0 = 1.0 + alpha;
            a1 = -2.0 * cosW;
            a2 = 1.0 - alpha;
            b0 = b2 = 1.0;
            b1 = a1;
            break;
        case Mode::LOW_SHELF: {
#if DSP_USE_VC
            const Vector amp = Vc::exp(0.5 * LOG2 * gain);
            const Vector ampPlus = amp + 1.0;
            const Vector ampMinus = amp - 1.0;
            const Vector ampPlusTimesCosW = ampPlus * cosW;
            const Vector ampMinusTimesCosW = ampMinus * cosW;
            const Vector alphaScaled = 2.0 * Vc::sqrt(amp) * alpha;
#else
            const Sample amp = exp2(0.5 * gain);
            const Sample ampPlus = amp + 1.0;
            const Sample ampMinus = amp - 1.0;
            const Sample ampPlusTimesCosW = ampPlus * cosW;
            const Sample ampMinusTimesCosW = ampMinus * cosW;
            const Sample alphaScaled = 2.0 * sqrt(amp) * alpha;
#endif
            a0 = ampPlus + ampMinusTimesCosW + alphaScaled;
            a1 = 2.0 * (ampMinus + ampPlusTimesCosW);
            a2 = ampPlus + ampMinusTimesCosW - alphaScaled;
            b0 = amp * (ampPlus - ampMinusTimesCosW + alphaScaled);
            b1 = -2.0 * amp * (ampMinus - ampPlusTimesCosW);
            b2 = amp * (ampPlus - ampMinusTimesCosW - alphaScaled);
        } break;
        case Mode::HIGH_SHELF: {
#if DSP_USE_VC
            const Vector amp = Vc::exp(0.5 * LOG2 * gain);
            const Vector ampPlus = amp + 1.0;
            const Vector ampMinus = amp - 1.0;
            const Vector ampPlusTimesCosW = ampPlus * cosW;
            const Vector ampMinusTimesCosW = ampMinus * cosW;
            const Vector alphaScaled = 2.0 * Vc::sqrt(amp) * alpha;
#else
            const Sample amp = exp2(0.5 * gain);
            const Sample ampPlus = amp + 1.0;
            const Sample ampMinus = amp - 1.0;
            const Sample ampPlusTimesCosW = ampPlus * cosW;
            const Sample ampMinusTimesCosW = ampMinus * cosW;
            const Sample alphaScaled = 2.0 * sqrt(amp) * alpha;
#endif
            a0 = ampPlus - ampMinusTimesCosW + alphaScaled;
            a1 = 2.0 * (ampMinus - ampPlusTimesCosW);
            a2 = ampPlus - ampMinusTimesCosW - alphaScaled;
            b0 = amp * (ampPlus + ampMinusTimesCosW + alphaScaled);
            b1 = -2.0 * amp * (ampMinus + ampPlusTimesCosW);
            b2 = amp * (ampPlus + ampMinusTimesCosW - alphaScaled);
        } break;
        case Mode::PEAK: {
#if DSP_USE_VC
            const Vector amp = Vc::exp(0.5 * LOG2 * gain);
            const Vector alphaMore = alpha * amp;
            const Vector alphaLess = alpha / amp;
#else
            const Sample amp = exp2(0.5 * gain);
            const Sample alphaMore = alpha * amp;
            const Sample alphaLess = alpha / amp;
#endif
            a0 = 1.0 + alphaLess;
            a1 = -2.0 * cosW;
            a2 = 1.0 - alphaLess;
            b0 = 1.0 + alphaMore;
            b1 = a1;
            b2 = 1.0 - alphaMore;
        } break;
        case Mode::ALL_PASS:
            a0 = b2 = 1.0 + alpha;
            a1 = b1 = -2.0 * cosW;
            a2 = b0 = 1.0 - alpha;
            break;
    }
}
