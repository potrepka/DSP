#include "GainEnvelope.h"

const unsigned int dsp::GainEnvelope::ATTACK = 1;
const unsigned int dsp::GainEnvelope::RELEASE = 2;

dsp::GainEnvelope::GainEnvelope() : Processor(Connection::Type::LINEAR, Connection::Type::LINEAR) {
    pushInput(Connection::Type::SECONDS);
    pushInput(Connection::Type::SECONDS);
}

std::shared_ptr<dsp::Unit::InputParameter> dsp::GainEnvelope::getAttack() {
    return getInput(ATTACK);
}

std::shared_ptr<dsp::Unit::InputParameter> dsp::GainEnvelope::getRelease() {
    return getInput(RELEASE);
}

void dsp::GainEnvelope::setNumChannelsNoLock(unsigned int numChannels) {
    Unit::setNumChannelsNoLock(numChannels);
    outputPrevious.resize(numChannels, 0);
}

void dsp::GainEnvelope::process() {
    Unit::process();
    for (unsigned int i = 0; i < getNumChannels(); i++) {
        std::vector<DSP_FLOAT> &inputBuffer = getInputSignal()->getChannel(i)->getBuffer();
        std::vector<DSP_FLOAT> &outputBuffer = getOutputSignal()->getChannel(i)->getBuffer();
        std::vector<DSP_FLOAT> &attackBuffer = getAttack()->getChannel(i)->getBuffer();
        std::vector<DSP_FLOAT> &releaseBuffer = getRelease()->getChannel(i)->getBuffer();
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
