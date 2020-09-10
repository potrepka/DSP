#include "Sequencer.h"

dsp::Sequencer::Sequencer(Type type, Space space)
        : Producer(type, space)
        , sequenceIndex(std::make_shared<Input>(Type::INTEGER))
        , index(std::make_shared<Input>(Type::INTEGER)) {
    getInputs().push_back(sequenceIndex);
    getInputs().push_back(index);
}

std::vector<std::shared_ptr<dsp::Buffer>> &dsp::Sequencer::getSequences() {
    return sequences;
}

std::shared_ptr<dsp::Input> dsp::Sequencer::getSequenceIndex() const {
    return sequenceIndex;
}

std::shared_ptr<dsp::Input> dsp::Sequencer::getIndex() const {
    return index;
}

void dsp::Sequencer::processNoLock() {
    if (sequences.size() > 0) {
        for (const auto &sequence : sequences) {
            if (sequence != nullptr) {
                sequence->lock();
            }
        }
        for (size_t channel = 0; channel < getNumChannels(); ++channel) {
            Sample *sequenceIndexChannel = getSequenceIndex()->getWrapper().getChannelPointer(channel);
            Sample *indexChannel = getIndex()->getWrapper().getChannelPointer(channel);
            Sample *outputChannel = getOutput()->getWrapper().getChannelPointer(channel);
            for (size_t sample = 0; sample < getNumSamples(); ++sample) {
                size_t p = static_cast<size_t>(sequenceIndexChannel[sample]);
                if (p < sequences.size() && sequences[p] != nullptr) {
                    size_t numChannels = sequences[p]->getNumChannels();
                    size_t numSamples = sequences[p]->getNumSamples();
                    if (numChannels > 0 && numSamples > 0) {
                        Sample *sequenceChannel = sequences[p]->getWrapper().getChannelPointer(channel % numChannels);
                        size_t index = static_cast<size_t>(indexChannel[sample]) % numSamples;
                        outputChannel[sample] = sequenceChannel[index + (index < 0) * numSamples];
                    } else {
                        outputChannel[sample] = sequences[p]->getDefaultValue();
                    }
                }
            }
        }
        for (const auto &sequence : sequences) {
            if (sequence != nullptr) {
                sequence->unlock();
            }
        }
    }
}
