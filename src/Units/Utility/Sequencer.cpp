#include "Sequencer.h"

dsp::Sequencer::Sequencer(Type type)
        : Generator(type)
        , RESET_TRIGGER(pushInput(Type::BINARY))
        , TRIGGER(pushInput(Type::BINARY)) {}

std::shared_ptr<dsp::Buffer> dsp::Sequencer::getSequence() const {
    return sequence;
}

void dsp::Sequencer::setSequence(std::shared_ptr<dsp::Buffer> sequence) {
    lock();
    this->sequence = sequence;
    unlock();
}

std::shared_ptr<dsp::Unit::InputParameter> dsp::Sequencer::getResetTrigger() const {
    return getInput(RESET_TRIGGER);
}

std::shared_ptr<dsp::Unit::InputParameter> dsp::Sequencer::getTrigger() const {
    return getInput(TRIGGER);
}

void dsp::Sequencer::setNumChannelsNoLock(unsigned int numChannels) {
    Unit::setNumChannelsNoLock(numChannels);
    memory.resize(numChannels, 0);
    index.resize(numChannels, 0);
}

void dsp::Sequencer::process() {
    Unit::process();
    if (sequence != nullptr && sequence->getNumChannels() > 0 && sequence->getBufferSize() > 0) {
        for (unsigned int i = 0; i < getNumChannels(); i++) {
            std::vector<DSP_FLOAT> &sequenceChannel = sequence->getChannel(i % sequence->getNumChannels());
            std::vector<DSP_FLOAT> &resetTriggerBuffer = getResetTrigger()->getChannel(i)->getBuffer();
            std::vector<DSP_FLOAT> &triggerBuffer = getTrigger()->getChannel(i)->getBuffer();
            std::vector<DSP_FLOAT> &outputBuffer = getOutputSignal()->getChannel(i)->getBuffer();
            for (unsigned int k = 0; k < getBufferSize(); k++) {
                if (resetTriggerBuffer[k]) {
                    index[i] = 0;
                }
                if (triggerBuffer[k]) {
                    index[i] %= sequence->getBufferSize();
                    memory[i] = index[i];
                    index[i]++;
                }
                outputBuffer[k] = sequenceChannel[memory[i]];
            }
        }
    }
}
