#include "MidiOutput.h"

dsp::MidiOutput::MidiOutput(Type type, std::shared_ptr<MidiData> outputMessages)
        : Consumer(type)
        , outputMessages(outputMessages)
        , previous(0.0) {}

std::shared_ptr<dsp::MidiData> dsp::MidiOutput::getOutputMessages() const {
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

std::function<void()> dsp::MidiOutput::processNote(int channel) {
    return [this, channel]() {
        for (int sample = 0; sample < getNumSamples(); ++sample) {
            for (int channel = 0; channel < getNumInputChannels(); ++channel) {
                Sample current = getInput()->getBlock().getSample(channel, sample);
                if (previous != current) {
                    const int previousNote = static_cast<int>(previous);
                    const int currentNote = static_cast<int>(current);
                    outputMessages->addEvent(MidiMessage::noteOff(channel, previousNote, uint8(0)), sample);
                    outputMessages->addEvent(MidiMessage::noteOn(channel, currentNote, uint8(127)), sample);
                    previous = current;
                }
            }
        }
    };
}

std::function<void()> dsp::MidiOutput::processNoteOn(int channel, std::unordered_set<int> noteSet) {
    return [this, channel, noteSet]() {
        processImpulse(noteSet, [channel](int note, Sample current) {
            return MidiMessage::noteOn(channel, note, static_cast<uint8>(unipolarToByte(current)));
        });
    };
}

std::function<void()> dsp::MidiOutput::processNoteOff(int channel, std::unordered_set<int> noteSet) {
    return [this, channel, noteSet]() {
        processImpulse(noteSet, [channel](int note) { return MidiMessage::noteOff(channel, note, uint8(0)); });
    };
}

std::function<void()> dsp::MidiOutput::processNotePressure(int channel, std::unordered_set<int> noteSet) {
    return [this, channel, noteSet]() {
        processContinuous(noteSet, [channel](int note, Sample current) {
            return MidiMessage::aftertouchChange(channel, note, static_cast<uint8>(unipolarToByte(current)));
        });
    };
}

std::function<void()> dsp::MidiOutput::processControl(int channel) {
    return [this, channel]() {
        processContinuous([channel](Sample current) {
            return MidiMessage::controllerEvent(channel, static_cast<int>(current), uint8(0));
        });
    };
}

std::function<void()> dsp::MidiOutput::processControlValue(int channel, std::unordered_set<int> controlSet) {
    return [this, channel, controlSet]() {
        processContinuous(controlSet, [channel](int control, Sample current) {
            return MidiMessage::controllerEvent(channel, control, static_cast<uint8>(unipolarToByte(current)));
        });
    };
}

std::function<void()> dsp::MidiOutput::processProgram(int channel) {
    return [this, channel]() {
        processContinuous(
                [channel](Sample current) { return MidiMessage::programChange(channel, static_cast<int>(current)); });
    };
}

std::function<void()> dsp::MidiOutput::processChannelPressure(int channel) {
    return [this, channel]() {
        processContinuous([channel](Sample current) {
            return MidiMessage::channelPressureChange(channel, static_cast<int>(unipolarToByte(current)));
        });
    };
}

std::function<void()> dsp::MidiOutput::processPitchBend(int channel) {
    return [this, channel]() {
        processContinuous([channel](Sample current) {
            return MidiMessage::channelPressureChange(channel, static_cast<int>(bipolarToShort(current)));
        });
    };
}

std::function<void()> dsp::MidiOutput::processAllNotesOff(int channel) {
    return [this, channel]() { processImpulse([channel]() { return MidiMessage::allNotesOff(channel); }); };
}

std::function<void()> dsp::MidiOutput::processSongPositionInQuarterNotes() {
    return [this]() {
        processContinuous(
                [](Sample current) { return MidiMessage::songPositionPointer(static_cast<int>(current * 4)); });
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

void dsp::MidiOutput::processImpulse(std::unordered_set<int> itemSet, std::function<MidiMessage(int, Sample)> publish) {
    for (int sample = 0; sample < getNumSamples(); ++sample) {
        for (int channel = 0; channel < getNumInputChannels(); ++channel) {
            const Sample current = getInput()->getBlock().getSample(channel, sample);
            if (current) {
                for (const auto item : itemSet) {
                    outputMessages->addEvent(publish(item, current), sample);
                }
            }
        }
    }
}

void dsp::MidiOutput::processImpulse(std::unordered_set<int> itemSet, std::function<MidiMessage(int)> publish) {
    for (int sample = 0; sample < getNumSamples(); ++sample) {
        for (int channel = 0; channel < getNumInputChannels(); ++channel) {
            if (getInput()->getBlock().getSample(channel, sample)) {
                for (const auto item : itemSet) {
                    outputMessages->addEvent(publish(item), sample);
                }
            }
        }
    }
}

void dsp::MidiOutput::processImpulse(std::function<MidiMessage(Sample)> publish) {
    for (int sample = 0; sample < getNumSamples(); ++sample) {
        for (int channel = 0; channel < getNumInputChannels(); ++channel) {
            const Sample current = getInput()->getBlock().getSample(channel, sample);
            if (current) {
                outputMessages->addEvent(publish(current), sample);
            }
        }
    }
}

void dsp::MidiOutput::processImpulse(std::function<MidiMessage()> publish) {
    for (int sample = 0; sample < getNumSamples(); ++sample) {
        for (int channel = 0; channel < getNumInputChannels(); ++channel) {
            if (getInput()->getBlock().getSample(channel, sample)) {
                outputMessages->addEvent(publish(), sample);
            }
        }
    }
}

void dsp::MidiOutput::processContinuous(std::unordered_set<int> itemSet,
                                        std::function<MidiMessage(int, Sample)> publish) {
    for (int sample = 0; sample < getNumSamples(); ++sample) {
        for (int channel = 0; channel < getNumInputChannels(); ++channel) {
            Sample current = getInput()->getBlock().getSample(channel, sample);
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
    for (int sample = 0; sample < getNumSamples(); ++sample) {
        for (int channel = 0; channel < getNumInputChannels(); ++channel) {
            Sample current = getInput()->getBlock().getSample(channel, sample);
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
