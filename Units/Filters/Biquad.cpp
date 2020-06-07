#include "Biquad.h"

const std::size_t dsp::Biquad::FREQUENCY = 1;
const std::size_t dsp::Biquad::Q = 2;
const std::size_t dsp::Biquad::GAIN = 3;

dsp::Biquad::Biquad() : Filter(dsp::Connection::Type::BIPOLAR) {
    pushInput(Connection::Type::HERTZ);
    pushInput(Connection::Type::RATIO, sqrt(0.5));
    pushInput(Connection::Type::DECIBELS);
}

void dsp::Biquad::setNumChannels(std::size_t size) {
    lock();
    for (const auto& input : inputs) {
        input->setNumChannels(size);
    }
    for (const auto& output : outputs) {
        output->setNumChannels(size);
    }
    std::size_t numChannels = getNumChannels();
    x1.resize(numChannels, 0);
    x2.resize(numChannels, 0);
    y1.resize(numChannels, 0);
    y2.resize(numChannels, 0);
    unlock();
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

void dsp::Biquad::process() {
    Filter::process();
    for (std::size_t i = 0; i < getNumChannels(); i++) {
        std::vector<DSP_FLOAT>& inputBuffer = getInputSignal()->getChannel(i)->getBuffer();
        std::vector<DSP_FLOAT>& outputBuffer = getOutputSignal()->getChannel(i)->getBuffer();
        std::vector<DSP_FLOAT>& frequencyBuffer = getFrequency()->getChannel(i)->getBuffer();
        std::vector<DSP_FLOAT>& qBuffer = getQ()->getChannel(i)->getBuffer();
        std::vector<DSP_FLOAT>& gainBuffer = getGain()->getChannel(i)->getBuffer();
        for (int k = 0; k < getBufferSize(); k++) {
            DSP_FLOAT& frequency = frequencyBuffer[k];
            DSP_FLOAT& q = qBuffer[k];
            DSP_FLOAT& gain = gainBuffer[k];
            calculateCoefficients(frequency, q, gain);
            outputBuffer[k] = (b0 * inputBuffer[k] + b1 * x1[i] + b2 * x2[i] - a1 * y1[i] - a2 * y2[i]) / a0;
            x2[i] = x1[i];
            x1[i] = inputBuffer[k];
            y2[i] = y1[i];
            y1[i] = outputBuffer[k];
        }
    }
}
