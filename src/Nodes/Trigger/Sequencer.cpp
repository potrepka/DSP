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
        for (int channel = 0; channel < getNumChannels(); ++channel) {
            Sample *sequenceIndexChannel = getSequenceIndex()->getBlock().getChannelPointer(channel);
            Sample *indexChannel = getIndex()->getBlock().getChannelPointer(channel);
            Sample *outputChannel = getOutput()->getBlock().getChannelPointer(channel);
            for (int sample = 0; sample < getNumSamples(); ++sample) {
                int p = static_cast<int>(sequenceIndexChannel[sample]);
                if (p >= 0 && p < sequences.size() && sequences[p] != nullptr) {
                    int numChannels = sequences[p]->getNumChannels();
                    int numSamples = sequences[p]->getNumSamples();
                    if (numChannels > 0 && numSamples > 0) {
                        Sample *sequenceChannel = sequences[p]->getBlock().getChannelPointer(channel % numChannels);
                        int index = static_cast<int>(indexChannel[sample]) % numSamples;
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
