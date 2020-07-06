#include "GainComputer.h"

const unsigned int dsp::GainComputer::THRESHOLD = 1;
const unsigned int dsp::GainComputer::COMPRESSION_RATIO = 2;
const unsigned int dsp::GainComputer::GATE_RATIO = 3;
const unsigned int dsp::GainComputer::KNEE = 4;

dsp::GainComputer::GainComputer(Connection::Space space)
        : Processor(Connection::Type::LINEAR, Connection::Type::LINEAR, space) {
    pushInput(Connection::Type::LINEAR, space);
    pushInput(Connection::Type::RATIO, space, 1.0);
    pushInput(Connection::Type::RATIO, space, 1.0);
    pushInput(Connection::Type::LINEAR, space);
}

std::shared_ptr<dsp::Unit::InputParameter> dsp::GainComputer::getThreshold() {
    return getInput(THRESHOLD);
}

std::shared_ptr<dsp::Unit::InputParameter> dsp::GainComputer::getCompressionRatio() {
    return getInput(COMPRESSION_RATIO);
}

std::shared_ptr<dsp::Unit::InputParameter> dsp::GainComputer::getGateRatio() {
    return getInput(GATE_RATIO);
}

std::shared_ptr<dsp::Unit::InputParameter> dsp::GainComputer::getKnee() {
    return getInput(KNEE);
}

void dsp::GainComputer::process() {
    Unit::process();
    for (unsigned int i = 0; i < getNumChannels(); i++) {
        std::vector<DSP_FLOAT> &inputBuffer = getInputSignal()->getChannel(i)->getBuffer();
        std::vector<DSP_FLOAT> &outputBuffer = getOutputSignal()->getChannel(i)->getBuffer();
        std::vector<DSP_FLOAT> &thresholdBuffer = getThreshold()->getChannel(i)->getBuffer();
        std::vector<DSP_FLOAT> &compressionRatioBuffer = getCompressionRatio()->getChannel(i)->getBuffer();
        std::vector<DSP_FLOAT> &gateRatioBuffer = getGateRatio()->getChannel(i)->getBuffer();
        std::vector<DSP_FLOAT> &kneeBuffer = getKnee()->getChannel(i)->getBuffer();
        for (unsigned int k = 0; k < getBufferSize(); k++) {
            DSP_FLOAT &input = inputBuffer[k];
            DSP_FLOAT &output = outputBuffer[k];
            DSP_FLOAT &threshold = thresholdBuffer[k];
            DSP_FLOAT &compressionRatio = compressionRatioBuffer[k];
            DSP_FLOAT &gateRatio = gateRatioBuffer[k];
            DSP_FLOAT &knee = kneeBuffer[k];

            DSP_FLOAT halfKnee = 0.5 * knee;
            DSP_FLOAT kneeLow = threshold - halfKnee;
            DSP_FLOAT kneeHigh = threshold + halfKnee;

            if (input > kneeLow && input < kneeHigh) {
                DSP_FLOAT t;
                if (gateRatio == 1.0) {
                    t = input / knee + 0.5;
                } else {
                    DSP_FLOAT overGate = halfKnee / gateRatio;
                    DSP_FLOAT diffGate = halfKnee - overGate;
                    t = (sqrt(halfKnee * overGate + input * diffGate) - overGate) / diffGate;
                }
                if (compressionRatio == 1.0) {
                    output = 2.0 * halfKnee * t - halfKnee - input;
                } else {
                    output = 2.0 * halfKnee * t - halfKnee - input + (halfKnee / compressionRatio - halfKnee) * t * t;
                }
            } else if (input > threshold) {
                if (input == std::numeric_limits<DSP_FLOAT>::infinity()) {
                    output = 0.0;
                } else {
                    output = (threshold - input) * (1.0 - 1.0 / compressionRatio);
                }
            } else {
                if (input == -std::numeric_limits<DSP_FLOAT>::infinity()) {
                    output = 0.0;
                } else {
                    output = (threshold - input) * (1.0 - gateRatio);
                }
            }
        }
    }
}
