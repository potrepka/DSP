#include "Sequencer.h"

dsp::Sequencer::Sequencer(Type type, Space space)
        : Generator(type, space)
        , index(pushInput(Type::INTEGER))
        , sequenceIndex(pushInput(Type::INTEGER)) {}

unsigned int dsp::Sequencer::getNumSequences() const {
    return getNumBuffers();
}

std::shared_ptr<dsp::Buffer> dsp::Sequencer::getSequence(unsigned int index) const {
    return getBuffer(index);
}

std::vector<std::shared_ptr<dsp::Buffer>> dsp::Sequencer::getSequences(unsigned int begin, unsigned int end) const {
    return getBuffers(begin, end);
}

void dsp::Sequencer::pushSequence(std::shared_ptr<Buffer> sequence) {
    lock();
    pushBuffer(sequence);
    unlock();
}

void dsp::Sequencer::pushSequences(std::vector<std::shared_ptr<Buffer>> sequences) {
    lock();
    pushBuffers(sequences);
    unlock();
}

void dsp::Sequencer::replaceSequence(std::shared_ptr<Buffer> sequence, std::shared_ptr<Buffer> replacement) {
    lock();
    replaceBuffer(sequence, replacement);
    unlock();
}

void dsp::Sequencer::removeSequence(std::shared_ptr<Buffer> sequence) {
    lock();
    removeBuffer(sequence);
    unlock();
}

void dsp::Sequencer::removeSequences(std::vector<std::shared_ptr<Buffer>> sequences) {
    lock();
    removeBuffers(sequences);
    unlock();
}

std::shared_ptr<dsp::InputParameter> dsp::Sequencer::getIndex() const {
    return index;
}

std::shared_ptr<dsp::InputParameter> dsp::Sequencer::getSequenceIndex() const {
    return sequenceIndex;
}

void dsp::Sequencer::process() {
    Unit::process();
    if (collection.size() > 0) {
        for (const auto &buffer : collection) {
            if (buffer != nullptr) {
                buffer->lock();
            }
        }
        for (unsigned int i = 0; i < getNumChannels(); ++i) {
            Array &indexBuffer = getIndex()->getChannel(i)->getBuffer();
            Array &sequenceIndexBuffer = getSequenceIndex()->getChannel(i)->getBuffer();
            Array &outputBuffer = getOutputSignal()->getChannel(i)->getBuffer();
            for (unsigned int k = 0; k < getBufferSize(); ++k) {
                Sample p = sequenceIndexBuffer[k];
                if (p >= 0 && p < collection.size() && collection[p] != nullptr) {
                    if (collection[p]->getNumChannels() > 0 && collection[p]->getBufferSize() > 0) {
                        Array &channel = collection[p]->getChannel(i % collection[p]->getNumChannels());
                        outputBuffer[k] = channel[wrap(indexBuffer[k], collection[p]->getBufferSize())];
                    } else {
                        outputBuffer[k] = collection[p]->getDefaultValue();
                    }
                }
            }
        }
        for (const auto &buffer : collection) {
            if (buffer != nullptr) {
                buffer->unlock();
            }
        }
    }
}
