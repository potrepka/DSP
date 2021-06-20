#include "MidiOutput.h"

dsp::MidiOutput::MidiOutput(std::shared_ptr<MidiBuffer> outputMessages, Type type)
        : Consumer(type)
        , outputMessages(outputMessages)
        , previous(0.0) {}

std::shared_ptr<dsp::MidiBuffer> dsp::MidiOutput::getOutputMessages() const {
    return outputMessages;
}

std::function<void()> dsp::MidiOutput::getProcessFunction() const {
    return processFunction;
}

void dsp::MidiOutput::setProcessFunction(std::function<void()> processFunction) {
    lock();
    this->processFunction = processFunction;
    previous = 0.0;
    unlock();
}

std::function<void()> dsp::MidiOutput::processNote(uint8 channel) {
    return [this, channel]() {
        for (size_t sample = 0; sample < getNumSamples(); ++sample) {
            for (size_t i = 0; i < getNumInputChannels(); ++i) {
                Sample current = getInput()->getWrapper().getSample(i, sample);
                if (previous != current) {
                    uint8 previousNote = static_cast<uint8>(previous);
                    uint8 currentNote = static_cast<uint8>(current);
                    outputMessages->addEvent(MidiMessage::noteOff(channel, previousNote, uint8(0)), sample);
                    outputMessages->addEvent(MidiMessage::noteOn(channel, currentNote, uint8(127)), sample);
                    previous = current;
                }
            }
        }
    };
}

std::function<void()> dsp::MidiOutput::processNoteOn(uint8 channel, std::unordered_set<uint8> noteSet) {
    return [this, channel, noteSet]() {
        processImpulse(noteSet, [channel](uint8 note, Sample current) {
            return MidiMessage::noteOn(channel, note, static_cast<uint8>(unipolarToByte(current)));
        });
    };
}

std::function<void()> dsp::MidiOutput::processNoteOff(uint8 channel, std::unordered_set<uint8> noteSet) {
    return [this, channel, noteSet]() {
        processImpulse(noteSet, [channel](uint8 note) { return MidiMessage::noteOff(channel, note, uint8(0)); });
    };
}

std::function<void()> dsp::MidiOutput::processNotePressure(uint8 channel, std::unordered_set<uint8> noteSet) {
    return [this, channel, noteSet]() {
        processContinuous(noteSet, [channel](uint8 note, Sample current) {
            return MidiMessage::aftertouchChange(channel, note, static_cast<uint8>(unipolarToByte(current)));
        });
    };
}

std::function<void()> dsp::MidiOutput::processControl(uint8 channel) {
    return [this, channel]() {
        processContinuous([channel](Sample current) {
            return MidiMessage::controllerEvent(channel, static_cast<uint8>(current), uint8(0));
        });
    };
}

std::function<void()> dsp::MidiOutput::processControlValue(uint8 channel, std::unordered_set<uint8> controlSet) {
    return [this, channel, controlSet]() {
        processContinuous(controlSet, [channel](uint8 control, Sample current) {
            return MidiMessage::controllerEvent(channel, control, static_cast<uint8>(unipolarToByte(current)));
        });
    };
}

std::function<void()> dsp::MidiOutput::processProgram(uint8 channel) {
    return [this, channel]() {
        processContinuous(
                [channel](Sample current) { return MidiMessage::programChange(channel, static_cast<uint8>(current)); });
    };
}

std::function<void()> dsp::MidiOutput::processChannelPressure(uint8 channel) {
    return [this, channel]() {
        processContinuous([channel](Sample current) {
            return MidiMessage::channelPressureChange(channel, static_cast<uint8>(unipolarToByte(current)));
        });
    };
}

std::function<void()> dsp::MidiOutput::processPitchBend(uint8 channel) {
    return [this, channel]() {
        processContinuous([channel](Sample current) {
            return MidiMessage::channelPressureChange(channel, static_cast<uint8>(bipolarToShort(current)));
        });
    };
}

std::function<void()> dsp::MidiOutput::processAllNotesOff(uint8 channel) {
    return [this, channel]() { processImpulse([channel]() { return MidiMessage::allNotesOff(channel); }); };
}

std::function<void()> dsp::MidiOutput::processSongPositionInQuarterNotes() {
    return [this]() {
        processContinuous(
                [](Sample current) { return MidiMessage::songPositionPointer(static_cast<uint8>(current * 4)); });
    };
}

std::function<void()> dsp::MidiOutput::processClock() {
    return [this]() { processImpulse([]() { return MidiMessage::midiStart(); }); };
}

std::function<void()> dsp::MidiOutput::processStart() {
    return [this]() { processImpulse([]() { return MidiMessage::midiStart(); }); };
}

std::function<void()> dsp::MidiOutput::processContinue() {
    return [this]() { processImpulse([]() { return MidiMessage::midiContinue(); }); };
}

std::function<void()> dsp::MidiOutput::processStop() {
    return [this]() { processImpulse([]() { return MidiMessage::midiStop(); }); };
}

void dsp::MidiOutput::processImpulse(std::unordered_set<uint8> itemSet,
                                     std::function<MidiMessage(uint8, Sample)> publish) {
    for (size_t sample = 0; sample < getNumSamples(); ++sample) {
        for (size_t channel = 0; channel < getNumInputChannels(); ++channel) {
            const Sample current = getInput()->getWrapper().getSample(channel, sample);
            if (current) {
                for (const auto item : itemSet) {
                    outputMessages->addEvent(publish(item, current), sample);
                }
            }
        }
    }
}

void dsp::MidiOutput::processImpulse(std::unordered_set<uint8> itemSet, std::function<MidiMessage(uint8)> publish) {
    for (size_t sample = 0; sample < getNumSamples(); ++sample) {
        for (size_t channel = 0; channel < getNumInputChannels(); ++channel) {
            if (getInput()->getWrapper().getSample(channel, sample)) {
                for (const auto item : itemSet) {
                    outputMessages->addEvent(publish(item), sample);
                }
            }
        }
    }
}

void dsp::MidiOutput::processImpulse(std::function<MidiMessage(Sample)> publish) {
    for (size_t sample = 0; sample < getNumSamples(); ++sample) {
        for (size_t channel = 0; channel < getNumInputChannels(); ++channel) {
            const Sample current = getInput()->getWrapper().getSample(channel, sample);
            if (current) {
                outputMessages->addEvent(publish(current), sample);
            }
        }
    }
}

void dsp::MidiOutput::processImpulse(std::function<MidiMessage()> publish) {
    for (size_t sample = 0; sample < getNumSamples(); ++sample) {
        for (size_t channel = 0; channel < getNumInputChannels(); ++channel) {
            if (getInput()->getWrapper().getSample(channel, sample)) {
                outputMessages->addEvent(publish(), sample);
            }
        }
    }
}

void dsp::MidiOutput::processContinuous(std::unordered_set<uint8> itemSet,
                                        std::function<MidiMessage(uint8, Sample)> publish) {
    for (size_t sample = 0; sample < getNumSamples(); ++sample) {
        for (size_t channel = 0; channel < getNumInputChannels(); ++channel) {
            Sample current = getInput()->getWrapper().getSample(channel, sample);
            if (previous != current) {
                for (const auto item : itemSet) {
                    outputMessages->addEvent(publish(item, current), sample);
                }
                previous = current;
            }
        }
    }
}

void dsp::MidiOutput::processContinuous(std::function<MidiMessage(Sample)> publish) {
    for (size_t sample = 0; sample < getNumSamples(); ++sample) {
        for (size_t channel = 0; channel < getNumInputChannels(); ++channel) {
            Sample current = getInput()->getWrapper().getSample(channel, sample);
            if (previous != current) {
                outputMessages->addEvent(publish(current), sample);
                previous = current;
            }
        }
    }
}

void dsp::MidiOutput::processNoLock() {
    if (processFunction) {
        processFunction();
    }
}
