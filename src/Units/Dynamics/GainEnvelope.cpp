#include "GainEnvelope.h"

dsp::GainEnvelope::GainEnvelope()
        : Processor(Type::LINEAR, Type::LINEAR)
        , ATTACK(pushInput(Type::SECONDS))
        , RELEASE(pushInput(Type::SECONDS)) {}

std::shared_ptr<dsp::InputParameter> dsp::GainEnvelope::getAttack() const {
    return getInput(ATTACK);
}

std::shared_ptr<dsp::InputParameter> dsp::GainEnvelope::getRelease() const {
    return getInput(RELEASE);
}

void dsp::GainEnvelope::setNumChannelsNoLock(unsigned int numChannels) {
    Unit::setNumChannelsNoLock(numChannels);
    outputPrevious.resize(numChannels, 0.0);
}

void dsp::GainEnvelope::process() {
    Unit::process();
    for (unsigned int i = 0; i < getNumChannels(); i++) {
        std::vector<DSP_FLOAT> &inputBuffer = getInputSignal()->getChannel(i)->getBuffer();
        std::vector<DSP_FLOAT> &attackBuffer = getAttack()->getChannel(i)->getBuffer();
        std::vector<DSP_FLOAT> &releaseBuffer = getRelease()->getChannel(i)->getBuffer();
        std::vector<DSP_FLOAT> &outputBuffer = getOutputSignal()->getChannel(i)->getBuffer();
        for (unsigned int k = 0; k < getBufferSize(); k++) {
            DSP_FLOAT samples;
            if (abs(inputBuffer[k]) > abs(outputPrevious[i])) {
                samples = attackBuffer[k] * getSampleRate();
            } else {
                samples = releaseBuffer[k] * getSampleRate();
            }
            outputPrevious[i] = inputBuffer[k] + pow(0.001, 1.0 / samples) * (outputPrevious[i] - inputBuffer[k]);
            outputBuffer[k] = outputPrevious[i];
        }
    }
}
