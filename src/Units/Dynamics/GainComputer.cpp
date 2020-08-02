#include "GainComputer.h"

dsp::GainComputer::GainComputer(Space space)
        : Processor(Type::LOGARITHMIC, Type::LOGARITHMIC, space)
        , threshold(pushInput(Type::LOGARITHMIC, space))
        , compressionRatio(pushInput(Type::RATIO, space, 1.0))
        , gateRatio(pushInput(Type::RATIO, space, 1.0))
        , knee(pushInput(Type::LOGARITHMIC, space)) {}

std::shared_ptr<dsp::InputParameter> dsp::GainComputer::getThreshold() const {
    return threshold;
}

std::shared_ptr<dsp::InputParameter> dsp::GainComputer::getCompressionRatio() const {
    return compressionRatio;
}

std::shared_ptr<dsp::InputParameter> dsp::GainComputer::getGateRatio() const {
    return gateRatio;
}

std::shared_ptr<dsp::InputParameter> dsp::GainComputer::getKnee() const {
    return knee;
}

void dsp::GainComputer::process() {
    Unit::process();
    for (unsigned int i = 0; i < getNumChannels(); ++i) {
        Array &inputBuffer = getInputSignal()->getChannel(i)->getBuffer();
        Array &thresholdBuffer = getThreshold()->getChannel(i)->getBuffer();
        Array &compressionRatioBuffer = getCompressionRatio()->getChannel(i)->getBuffer();
        Array &gateRatioBuffer = getGateRatio()->getChannel(i)->getBuffer();
        Array &kneeBuffer = getKnee()->getChannel(i)->getBuffer();
        Array &outputBuffer = getOutputSignal()->getChannel(i)->getBuffer();
        for (unsigned int k = 0; k < getBufferSize(); ++k) {
            Sample &input = inputBuffer[k];
            Sample &output = outputBuffer[k];
            Sample &threshold = thresholdBuffer[k];
            Sample &compressionRatio = compressionRatioBuffer[k];
            Sample &gateRatio = gateRatioBuffer[k];
            Sample &knee = kneeBuffer[k];

            Sample halfKnee = 0.5 * knee;
            Sample kneeLow = threshold - halfKnee;
            Sample kneeHigh = threshold + halfKnee;

            if (input > kneeLow && input < kneeHigh) {
                Sample t;
                if (gateRatio == 1.0) {
                    t = input / knee + 0.5;
                } else {
                    Sample overGate = halfKnee / gateRatio;
                    Sample diffGate = halfKnee - overGate;
                    t = (sqrt(halfKnee * overGate + input * diffGate) - overGate) / diffGate;
                }
                if (compressionRatio == 1.0) {
                    output = 2.0 * halfKnee * t - halfKnee - input;
                } else {
                    output = 2.0 * halfKnee * t - halfKnee - input + (halfKnee / compressionRatio - halfKnee) * t * t;
                }
            } else if (input > threshold) {
                if (input == std::numeric_limits<Sample>::infinity()) {
                    output = 0.0;
                } else {
                    output = (threshold - input) * (1.0 - 1.0 / compressionRatio);
                }
            } else {
                if (input == -std::numeric_limits<Sample>::infinity()) {
                    output = 0.0;
                } else {
                    output = (threshold - input) * (1.0 - gateRatio);
                }
            }
        }
    }
}
