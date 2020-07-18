#include "Biquad.h"

dsp::Biquad::Biquad()
        : Processor(dsp::Type::BIPOLAR, dsp::Type::BIPOLAR)
        , frequency(pushInput(Type::HERTZ))
        , q(pushInput(Type::RATIO, Space::TIME, 1.0))
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

std::shared_ptr<dsp::InputParameter> dsp::Biquad::getQ() const {
    return q;
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
    for (unsigned int i = 0; i < getNumChannels(); i++) {
        std::vector<DSP_FLOAT> &inputBuffer = getInputSignal()->getChannel(i)->getBuffer();
        std::vector<DSP_FLOAT> &frequencyBuffer = getFrequency()->getChannel(i)->getBuffer();
        std::vector<DSP_FLOAT> &qBuffer = getQ()->getChannel(i)->getBuffer();
        std::vector<DSP_FLOAT> &gainBuffer = getGain()->getChannel(i)->getBuffer();
        std::vector<DSP_FLOAT> &outputBuffer = getOutputSignal()->getChannel(i)->getBuffer();
        for (unsigned int k = 0; k < getBufferSize(); k++) {
            if (qBuffer[k] == 0.0) {
                outputBuffer[k] = 0.0;
            } else {
                calculateCoefficients(frequencyBuffer[k], qBuffer[k], gainBuffer[k]);
                outputBuffer[k] = (b0 * inputBuffer[k] + b1 * x1[i] + b2 * x2[i] - a1 * y1[i] - a2 * y2[i]) / a0;
            }
            x2[i] = x1[i];
            x1[i] = inputBuffer[k];
            y2[i] = y1[i];
            y1[i] = outputBuffer[k];
        }
    }
}

void dsp::Biquad::calculateCoefficients(const DSP_FLOAT &frequency, const DSP_FLOAT &q, const DSP_FLOAT &gain) {
    const double omega = TAU * frequency * getOneOverSampleRate();
    const double sinW = sin(omega);
    const double cosW = cos(omega);
    const double alpha = sinW / (SQRT2 * q);

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
            const double amp = exp2(0.5 * gain);
            const double ampPlus = amp + 1.0;
            const double ampMinus = amp - 1.0;
            const double alphaScaled = 2.0 * sqrt(amp) * alpha;
            const double ampPlusTimesCosW = ampPlus * cosW;
            const double ampMinusTimesCosW = ampMinus * cosW;
            a0 = ampPlus + ampMinusTimesCosW + alphaScaled;
            a1 = 2.0 * (ampMinus + ampPlusTimesCosW);
            a2 = ampPlus + ampMinusTimesCosW - alphaScaled;
            b0 = amp * (ampPlus - ampMinusTimesCosW + alphaScaled);
            b1 = -2.0 * amp * (ampMinus - ampPlusTimesCosW);
            b2 = amp * (ampPlus - ampMinusTimesCosW - alphaScaled);
        } break;
        case Mode::HIGH_SHELF: {
            const double amp = exp2(0.5 * gain);
            const double ampPlus = amp + 1.0;
            const double ampMinus = amp - 1.0;
            const double alphaScaled = 2.0 * sqrt(amp) * alpha;
            const double ampPlusTimesCosW = ampPlus * cosW;
            const double ampMinusTimesCosW = ampMinus * cosW;
            a0 = ampPlus - ampMinusTimesCosW + alphaScaled;
            a1 = 2.0 * (ampMinus - ampPlusTimesCosW);
            a2 = ampPlus - ampMinusTimesCosW - alphaScaled;
            b0 = amp * (ampPlus + ampMinusTimesCosW + alphaScaled);
            b1 = -2.0 * amp * (ampMinus + ampPlusTimesCosW);
            b2 = amp * (ampPlus + ampMinusTimesCosW - alphaScaled);
        } break;
        case Mode::PEAK: {
            const double amp = exp2(0.5 * gain);
            const double alphaMore = alpha * amp;
            const double alphaLess = alpha / amp;
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
