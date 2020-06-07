#include "Envelope.h"

const std::size_t dsp::Envelope::ATTACK = 1;
const std::size_t dsp::Envelope::RELEASE = 2;

dsp::Envelope::Envelope() : Filter(Connection::Type::UNIPOLAR) {
    pushInput(Connection::Type::SECONDS);
    pushInput(Connection::Type::SECONDS);
}

std::shared_ptr<dsp::Unit::InputParameter> dsp::Envelope::getAttack() {
    return getInput(ATTACK);
}

std::shared_ptr<dsp::Unit::InputParameter> dsp::Envelope::getRelease() {
    return getInput(RELEASE);
}

void dsp::Envelope::setNumChannels(std::size_t size) {
    lock();
    for (const auto &input : inputs) {
        input->setNumChannels(size);
    }
    for (const auto &output : outputs) {
        output->setNumChannels(size);
    }
    outputPrevious.resize(size, 0);
    unlock();
}

void dsp::Envelope::process() {
    Generator::process();
    for (std::size_t i = 0; i < getNumChannels(); i++) {
        std::vector<DSP_FLOAT> &inputBuffer = getInputSignal()->getChannel(i)->getBuffer();
        std::vector<DSP_FLOAT> &outputBuffer = getOutputSignal()->getChannel(i)->getBuffer();
        std::vector<DSP_FLOAT> &attackBuffer = getAttack()->getChannel(i)->getBuffer();
        std::vector<DSP_FLOAT> &releaseBuffer = getRelease()->getChannel(i)->getBuffer();
        for (int k = 0; k < getBufferSize(); k++) {
            DSP_FLOAT samples;
            if (abs(inputBuffer[k]) > abs(outputPrevious[i])) {
                samples = attackBuffer[k] * getSampleRate();
            } else {
                samples = releaseBuffer[k] * getSampleRate();
            }
            outputPrevious[i] += (inputBuffer[k] - outputPrevious[i]) * pow(0.001, 1.0 / samples);
            outputBuffer[k] = outputPrevious[i];
        }
    }
}
