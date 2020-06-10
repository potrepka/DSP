#include "Biquad.h"

const std::size_t dsp::Biquad::FREQUENCY = 1;
const std::size_t dsp::Biquad::Q = 2;
const std::size_t dsp::Biquad::GAIN = 3;

dsp::Biquad::Biquad()
        : Processor(dsp::Connection::Type::BIPOLAR, dsp::Connection::Type::BIPOLAR), mode(Mode::LOW_PASS) {
    pushInput(Connection::Type::HERTZ);
    pushInput(Connection::Type::RATIO, sqrt(0.5));
    pushInput(Connection::Type::DECIBELS);
}

void dsp::Biquad::setNumChannels(std::size_t numChannels) {
    lock();
    Unit::setNumChannelsNoLock(numChannels);
    x1.resize(numChannels, 0);
    x2.resize(numChannels, 0);
    y1.resize(numChannels, 0);
    y2.resize(numChannels, 0);
    unlock();
}

dsp::Biquad::Mode dsp::Biquad::getMode() {
    return mode;
}

void dsp::Biquad::setMode(Mode mode) {
    this->mode = mode;
}

std::shared_ptr<dsp::Unit::InputParameter> dsp::Biquad::getFrequency() {
    return getInput(FREQUENCY);
}

std::shared_ptr<dsp::Unit::InputParameter> dsp::Biquad::getQ() {
    return getInput(Q);
}

std::shared_ptr<dsp::Unit::InputParameter> dsp::Biquad::getGain() {
    return getInput(GAIN);
}

void dsp::Biquad::calculateCoefficients(const DSP_FLOAT &frequency, const DSP_FLOAT &q, const DSP_FLOAT &gain) {
    double omega = TAU * frequency / getSampleRate();
    double sinW = sin(omega);
    double cosW = cos(omega);
    double alpha = sinW / (2.0 * q);

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
            double amp = pow(10.0, gain * 0.025);
            double ampPlus = amp + 1.0;
            double ampMinus = amp - 1.0;
            double alphaScaled = 2.0 * sqrt(amp) * alpha;
            double ampPlusTimesCosW = ampPlus * cosW;
            double ampMinusTimesCosW = ampMinus * cosW;
            a0 = ampPlus + ampMinusTimesCosW + alphaScaled;
            a1 = 2.0 * (ampMinus + ampPlusTimesCosW);
            a2 = ampPlus + ampMinusTimesCosW - alphaScaled;
            b0 = amp * (ampPlus - ampMinusTimesCosW + alphaScaled);
            b1 = -2.0 * amp * (ampMinus - ampPlusTimesCosW);
            b2 = amp * (ampPlus - ampMinusTimesCosW - alphaScaled);
        } break;
        case Mode::HIGH_SHELF: {
            double amp = pow(10.0, gain * 0.025);
            double ampPlus = amp + 1.0;
            double ampMinus = amp - 1.0;
            double alphaScaled = 2.0 * sqrt(amp) * alpha;
            double ampPlusTimesCosW = ampPlus * cosW;
            double ampMinusTimesCosW = ampMinus * cosW;
            a0 = ampPlus - ampMinusTimesCosW + alphaScaled;
            a1 = 2.0 * (ampMinus - ampPlusTimesCosW);
            a2 = ampPlus - ampMinusTimesCosW - alphaScaled;
            b0 = amp * (ampPlus + ampMinusTimesCosW + alphaScaled);
            b1 = -2.0 * amp * (ampMinus + ampPlusTimesCosW);
            b2 = amp * (ampPlus + ampMinusTimesCosW - alphaScaled);
        } break;
        case Mode::PEAK: {
            double amp = pow(10.0, gain * 0.025);
            double alphaMore = alpha * amp;
            double alphaLess = alpha / amp;
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

void dsp::Biquad::process() {
    Processor::process();
    for (std::size_t i = 0; i < getNumChannels(); i++) {
        std::vector<DSP_FLOAT> &inputBuffer = getInputSignal()->getChannel(i)->getBuffer();
        std::vector<DSP_FLOAT> &outputBuffer = getOutputSignal()->getChannel(i)->getBuffer();
        std::vector<DSP_FLOAT> &frequencyBuffer = getFrequency()->getChannel(i)->getBuffer();
        std::vector<DSP_FLOAT> &qBuffer = getQ()->getChannel(i)->getBuffer();
        std::vector<DSP_FLOAT> &gainBuffer = getGain()->getChannel(i)->getBuffer();
        for (int k = 0; k < getBufferSize(); k++) {
            DSP_FLOAT &frequency = frequencyBuffer[k];
            DSP_FLOAT &q = qBuffer[k];
            DSP_FLOAT &gain = gainBuffer[k];
            if (q == 0) {
                outputBuffer[k] = 0;
            } else {
                calculateCoefficients(frequency, q, gain);
                outputBuffer[k] = (b0 * inputBuffer[k] + b1 * x1[i] + b2 * x2[i] - a1 * y1[i] - a2 * y2[i]) / a0;
            }
            x2[i] = x1[i];
            x1[i] = inputBuffer[k];
            y2[i] = y1[i];
            y1[i] = outputBuffer[k];
        }
    }
}
