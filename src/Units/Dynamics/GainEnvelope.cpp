#include "GainEnvelope.h"

dsp::GainEnvelope::GainEnvelope()
        : Processor(Type::LINEAR, Type::LINEAR)
        , attack(pushInput(Type::SECONDS))
        , release(pushInput(Type::SECONDS)) {}

std::shared_ptr<dsp::InputParameter> dsp::GainEnvelope::getAttack() const {
    return attack;
}

std::shared_ptr<dsp::InputParameter> dsp::GainEnvelope::getRelease() const {
    return release;
}

void dsp::GainEnvelope::setNumChannelsNoLock(unsigned int numChannels) {
    Unit::setNumChannelsNoLock(numChannels);
    outputPrevious.resize(numChannels, 0.0);
}

void dsp::GainEnvelope::process() {
    Unit::process();
    for (unsigned int i = 0; i < getNumChannels(); i++) {
        std::vector<Sample> &inputBuffer = getInputSignal()->getChannel(i)->getBuffer();
        std::vector<Sample> &attackBuffer = getAttack()->getChannel(i)->getBuffer();
        std::vector<Sample> &releaseBuffer = getRelease()->getChannel(i)->getBuffer();
        std::vector<Sample> &outputBuffer = getOutputSignal()->getChannel(i)->getBuffer();
        for (unsigned int k = 0; k < getBufferSize(); k++) {
            Sample samples;
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
