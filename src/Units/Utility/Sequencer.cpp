#include "Sequencer.h"

const unsigned int dsp::Sequencer::RESET_TRIGGER = 0;
const unsigned int dsp::Sequencer::TRIGGER = 1;

dsp::Sequencer::Sequencer() : Generator(Type::INTEGER), sequence(nullptr), index(0) {
    pushInput(Type::BINARY);
    pushInput(Type::BINARY);
}

std::vector<unsigned int> &dsp::Sequencer::getSequence() {
    return *sequence;
}

void dsp::Sequencer::setSequence(std::vector<unsigned int> &sequence) {
    lock();
    this->sequence = &sequence;
    unlock();
}

void dsp::Sequencer::removeSequence() {
    lock();
    sequence = nullptr;
    unlock();
}

std::shared_ptr<dsp::Unit::InputParameter> dsp::Sequencer::getResetTrigger() {
    return getInput(RESET_TRIGGER);
}

std::shared_ptr<dsp::Unit::InputParameter> dsp::Sequencer::getTrigger() {
    return getInput(TRIGGER);
}

void dsp::Sequencer::setNumChannelsNoLock(unsigned int numChannels) {
    Unit::setNumChannelsNoLock(numChannels);
    memory.resize(numChannels, 0);
    index.resize(numChannels, 0);
}

void dsp::Sequencer::process() {
    Unit::process();
    if (sequence != nullptr && sequence->size() > 0) {
        for (unsigned int i = 0; i < getNumChannels(); i++) {
            std::vector<DSP_FLOAT> &outputBuffer = getOutputSignal()->getChannel(i)->getBuffer();
            std::vector<DSP_FLOAT> &resetTriggerBuffer = getResetTrigger()->getChannel(i)->getBuffer();
            std::vector<DSP_FLOAT> &triggerBuffer = getTrigger()->getChannel(i)->getBuffer();
            for (unsigned int k = 0; k < getBufferSize(); k++) {
                if (resetTriggerBuffer[k]) {
                    index[i] = 0;
                }
                if (triggerBuffer[k]) {
                    memory[i] = index[i];
                    index[i] = (index[i] + 1) % sequence->size();
                }
                outputBuffer[k] = memory[i];
            }
        }
    }
}
