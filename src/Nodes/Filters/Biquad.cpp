#include "Biquad.h"

#include "Biquad.h"

dsp::Biquad::Biquad()
        : Transformer(Type::RATIO, Type::RATIO)
        , mode(Mode::LOW_PASS)
        , resetTrigger(std::make_shared<Input>(Type::INTEGER))
        , frequency(std::make_shared<Input>(Type::HERTZ))
        , resonance(std::make_shared<Input>(Type::RATIO, Space::TIME, 1.0))
        , gain(std::make_shared<Input>(Type::LOGARITHMIC)) {
    getInputs().push_back(resetTrigger);
    getInputs().push_back(frequency);
    getInputs().push_back(resonance);
    getInputs().push_back(gain);
}

dsp::Biquad::Mode dsp::Biquad::getMode() const {
    return mode;
}

void dsp::Biquad::setMode(Mode mode) {
    lock();
    this->mode = mode;
    unlock();
}

std::shared_ptr<dsp::Input> dsp::Biquad::getResetTrigger() const {
    return resetTrigger;
}

std::shared_ptr<dsp::Input> dsp::Biquad::getFrequency() const {
    return frequency;
}

std::shared_ptr<dsp::Input> dsp::Biquad::getResonance() const {
    return resonance;
}

std::shared_ptr<dsp::Input> dsp::Biquad::getGain() const {
    return gain;
}

void dsp::Biquad::setNumOutputChannelsNoLock(size_t numChannels) {
    Node::setNumOutputChannelsNoLock(numChannels);
    x1.resize(numChannels, 0.0);
    x2.resize(numChannels, 0.0);
    y1.resize(numChannels, 0.0);
    y2.resize(numChannels, 0.0);
}

void dsp::Biquad::processNoLock() {
    for (size_t channel = 0; channel < getNumChannels(); ++channel) {
        Sample *inputChannel = getInput()->getWrapper().getChannelPointer(channel);
        Sample *resetTriggerChannel = getResetTrigger()->getWrapper().getChannelPointer(channel);
        Sample *frequencyChannel = getFrequency()->getWrapper().getChannelPointer(channel);
        Sample *resonanceChannel = getResonance()->getWrapper().getChannelPointer(channel);
        Sample *gainChannel = getGain()->getWrapper().getChannelPointer(channel);
        Sample *outputChannel = getOutput()->getWrapper().getChannelPointer(channel);
        for (size_t sample = 0; sample < getNumSamples(); ++sample) {
            if (resetTriggerChannel[sample]) {
                x1[channel] = 0.0;
                x2[channel] = 0.0;
                y1[channel] = 0.0;
                y2[channel] = 0.0;
            }
            calculateCoefficients(frequencyChannel[sample], resonanceChannel[sample], gainChannel[sample]);
            outputChannel[sample] = (b0 * inputChannel[sample] + b1 * x1[channel] + b2 * x2[channel] -
                                     a1 * y1[channel] - a2 * y2[channel]) /
                                    a0;
            x2[channel] = x1[channel];
            x1[channel] = inputChannel[sample];
            y2[channel] = y1[channel];
            y1[channel] = outputChannel[sample];
        }
    }
}

void dsp::Biquad::calculateCoefficients(const Sample &frequency, const Sample &resonance, const Sample &gain) {
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
            const Sample amp = exp2(0.5 * gain);
            const Sample ampPlus = amp + 1.0;
            const Sample ampMinus = amp - 1.0;
            const Sample ampPlusTimesCosW = ampPlus * cosW;
            const Sample ampMinusTimesCosW = ampMinus * cosW;
            const Sample alphaScaled = 2.0 * sqrt(amp) * alpha;
            a0 = ampPlus + ampMinusTimesCosW + alphaScaled;
            a1 = 2.0 * (ampMinus + ampPlusTimesCosW);
            a2 = ampPlus + ampMinusTimesCosW - alphaScaled;
            b0 = amp * (ampPlus - ampMinusTimesCosW + alphaScaled);
            b1 = -2.0 * amp * (ampMinus - ampPlusTimesCosW);
            b2 = amp * (ampPlus - ampMinusTimesCosW - alphaScaled);
        } break;
        case Mode::HIGH_SHELF: {
            const Sample amp = exp2(0.5 * gain);
            const Sample ampPlus = amp + 1.0;
            const Sample ampMinus = amp - 1.0;
            const Sample ampPlusTimesCosW = ampPlus * cosW;
            const Sample ampMinusTimesCosW = ampMinus * cosW;
            const Sample alphaScaled = 2.0 * sqrt(amp) * alpha;
            a0 = ampPlus - ampMinusTimesCosW + alphaScaled;
            a1 = 2.0 * (ampMinus - ampPlusTimesCosW);
            a2 = ampPlus - ampMinusTimesCosW - alphaScaled;
            b0 = amp * (ampPlus + ampMinusTimesCosW + alphaScaled);
            b1 = -2.0 * amp * (ampMinus + ampPlusTimesCosW);
            b2 = amp * (ampPlus + ampMinusTimesCosW - alphaScaled);
        } break;
        case Mode::PEAK: {
            const Sample amp = exp2(0.5 * gain);
            const Sample alphaMore = alpha * amp;
            const Sample alphaLess = alpha / amp;
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
