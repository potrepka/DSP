#include "Biquad.h"

#include "Biquad.h"

dsp::Biquad::Biquad()
        : Transformer(Type::RATIO, Type::RATIO)
        , mode(Mode::LOW_PASS)
        , frequency(std::make_shared<Input>(Type::HERTZ))
        , resonance(std::make_shared<Input>(Type::RATIO, Space::TIME, 1.0))
        , amplitude(std::make_shared<Input>(Type::RATIO, Space::TIME, 1.0))
        , reset(std::make_shared<Input>(Type::BOOLEAN)) {
    getInputs().push_back(frequency);
    getInputs().push_back(resonance);
    getInputs().push_back(amplitude);
    getInputs().push_back(reset);
}

dsp::Biquad::Mode dsp::Biquad::getMode() const {
    return mode;
}

void dsp::Biquad::setMode(Mode mode) {
    lock();
    this->mode = mode;
    unlock();
}

std::shared_ptr<dsp::Input> dsp::Biquad::getFrequency() const {
    return frequency;
}

std::shared_ptr<dsp::Input> dsp::Biquad::getResonance() const {
    return resonance;
}

std::shared_ptr<dsp::Input> dsp::Biquad::getAmplitude() const {
    return amplitude;
}

std::shared_ptr<dsp::Input> dsp::Biquad::getReset() const {
    return reset;
}

void dsp::Biquad::getMagnitudeAndPhaseResponse(size_t channel,
                                               Sample frequency,
                                               Sample &magnitude,
                                               Sample &phase) const {
    DSP_ASSERT(channel < getNumChannels());
    const Sample omega = TAU * frequency * getOneOverSampleRate();
    Sample a0 = aa0[channel];
    Sample a1 = aa1[channel];
    Sample a2 = aa2[channel];
    Sample b0 = bb0[channel];
    Sample b1 = bb1[channel];
    Sample b2 = bb2[channel];
    const Sample sinW = sin(omega);
    const Sample cosW = cos(omega);
    const Sample sin2W = sin(2.0 * omega);
    const Sample cos2W = cos(2.0 * omega);
    Sample a = b0 + b1 * cosW + b2 * cos2W;
    Sample b = -b1 * sinW - b2 * sin2W;
    DSP_ASSERT(channel < getNumChannels());
    Sample c = a0 + a1 * cosW + a2 * cos2W;
    Sample d = -a1 * sinW - a2 * sin2W;
    Sample denominator = c * c + d * d;
    Sample real = (a * c + b * d) / denominator;
    Sample imaginary = (b * c - a * d) / denominator;
    magnitude = sqrt(real * real + imaginary * imaginary);
    Sample bipolar = ONE_OVER_TAU * atan2(imaginary, real);
    phase = bipolar < 0.0 ? bipolar + 1.0 : bipolar;
}

void dsp::Biquad::setNumOutputChannelsNoLock(size_t numChannels) {
    Node::setNumOutputChannelsNoLock(numChannels);
    xx1.resize(numChannels, 0.0);
    xx2.resize(numChannels, 0.0);
    yy1.resize(numChannels, 0.0);
    yy2.resize(numChannels, 0.0);
    aa0.resize(numChannels, 0.0);
    aa1.resize(numChannels, 0.0);
    aa2.resize(numChannels, 0.0);
    bb0.resize(numChannels, 0.0);
    bb1.resize(numChannels, 0.0);
    bb2.resize(numChannels, 0.0);
}

void dsp::Biquad::processNoLock() {
    for (size_t channel = 0; channel < getNumChannels(); ++channel) {
        Sample *inputChannel = getInput()->getWrapper().getChannelPointer(channel);
        Sample *frequencyChannel = getFrequency()->getWrapper().getChannelPointer(channel);
        Sample *resonanceChannel = getResonance()->getWrapper().getChannelPointer(channel);
        Sample *amplitudeChannel = getAmplitude()->getWrapper().getChannelPointer(channel);
        Sample *resetChannel = getReset()->getWrapper().getChannelPointer(channel);
        Sample *outputChannel = getOutput()->getWrapper().getChannelPointer(channel);
        Sample &x1 = xx1[channel];
        Sample &x2 = xx2[channel];
        Sample &y1 = yy1[channel];
        Sample &y2 = yy2[channel];
        Sample &a0 = aa0[channel];
        Sample &a1 = aa1[channel];
        Sample &a2 = aa2[channel];
        Sample &b0 = bb0[channel];
        Sample &b1 = bb1[channel];
        Sample &b2 = bb2[channel];
        for (size_t sample = 0; sample < getNumSamples(); ++sample) {
            if (resetChannel[sample]) {
                x1 = 0.0;
                x2 = 0.0;
                y1 = 0.0;
                y2 = 0.0;
            }
            calculateCoefficients(frequencyChannel[sample],
                                  resonanceChannel[sample],
                                  amplitudeChannel[sample],
                                  a0,
                                  a1,
                                  a2,
                                  b0,
                                  b1,
                                  b2);
            outputChannel[sample] = (b0 * inputChannel[sample] + b1 * x1 + b2 * x2 - a1 * y1 - a2 * y2) / a0;
            x2 = x1;
            x1 = inputChannel[sample];
            y2 = y1;
            y1 = outputChannel[sample];
        }
    }
}

void dsp::Biquad::calculateCoefficients(const Sample &frequency,
                                        const Sample &resonance,
                                        const Sample &amplitude,
                                        Sample &a0,
                                        Sample &a1,
                                        Sample &a2,
                                        Sample &b0,
                                        Sample &b1,
                                        Sample &b2) const {
    const Sample omega = TAU * frequency * getOneOverSampleRate();
    const Sample sinW = sin(omega);
    const Sample cosW = cos(omega);
    const Sample alpha = sinW / (SQRT2 * resonance);
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
            const Sample amp = sqrt(amplitude);
            const Sample ampPlusOne = amp + 1.0;
            const Sample ampMinusOne = amp - 1.0;
            const Sample ampPlusOneTimesCosW = ampPlusOne * cosW;
            const Sample ampMinusOneTimesCosW = ampMinusOne * cosW;
            const Sample alphaTimesTwoRootAmp = 2.0 * sqrt(amp) * alpha;
            a0 = ampPlusOne + ampMinusOneTimesCosW + alphaTimesTwoRootAmp;
            a1 = 2.0 * (ampMinusOne + ampPlusOneTimesCosW);
            a2 = ampPlusOne + ampMinusOneTimesCosW - alphaTimesTwoRootAmp;
            b0 = amp * (ampPlusOne - ampMinusOneTimesCosW + alphaTimesTwoRootAmp);
            b1 = -2.0 * amp * (ampMinusOne - ampPlusOneTimesCosW);
            b2 = amp * (ampPlusOne - ampMinusOneTimesCosW - alphaTimesTwoRootAmp);
        } break;
        case Mode::HIGH_SHELF: {
            const Sample amp = sqrt(amplitude);
            const Sample ampPlusOne = amp + 1.0;
            const Sample ampMinusOne = amp - 1.0;
            const Sample ampPlusOneTimesCosW = ampPlusOne * cosW;
            const Sample ampMinusOneTimesCosW = ampMinusOne * cosW;
            const Sample alphaTimesTwoRootAmp = 2.0 * sqrt(amp) * alpha;
            a0 = ampPlusOne - ampMinusOneTimesCosW + alphaTimesTwoRootAmp;
            a1 = 2.0 * (ampMinusOne - ampPlusOneTimesCosW);
            a2 = ampPlusOne - ampMinusOneTimesCosW - alphaTimesTwoRootAmp;
            b0 = amp * (ampPlusOne + ampMinusOneTimesCosW + alphaTimesTwoRootAmp);
            b1 = -2.0 * amp * (ampMinusOne + ampPlusOneTimesCosW);
            b2 = amp * (ampPlusOne + ampMinusOneTimesCosW - alphaTimesTwoRootAmp);
        } break;
        case Mode::PEAK: {
            const Sample amp = sqrt(amplitude);
            const Sample alphaTimesAmp = alpha * amp;
            const Sample alphaOverAmp = alpha / amp;
            a0 = 1.0 + alphaOverAmp;
            a1 = -2.0 * cosW;
            a2 = 1.0 - alphaOverAmp;
            b0 = 1.0 + alphaTimesAmp;
            b1 = a1;
            b2 = 1.0 - alphaTimesAmp;
        } break;
        case Mode::ALL_PASS:
            a0 = b2 = 1.0 + alpha;
            a1 = b1 = -2.0 * cosW;
            a2 = b0 = 1.0 - alpha;
            break;
    }
}
