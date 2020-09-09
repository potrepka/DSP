#include "MidiInput.h"

dsp::MidiInput::MidiInput(Type type, std::shared_ptr<MidiData> inputMessages)
        : Producer(type)
        , inputMessages(inputMessages)
        , previous(0.0) {}

std::shared_ptr<dsp::MidiData> dsp::MidiInput::getInputMessages() const {
    return inputMessages;
}

std::function<void()> dsp::MidiInput::getProcessFunction() const {
    return processFunction;
}

void dsp::MidiInput::setProcessFunction(std::function<void()> processFunction) {
    lock();
    this->processFunction = processFunction;
    previous = 0.0;
    unlock();
}

std::function<void()> dsp::MidiInput::processNote(int channel, std::unordered_set<int> noteSet) {
    return [this, channel, noteSet]() {
        processContinuous(
                [channel, noteSet](MidiMessage message) {
                    return message.isNoteOn() && (channel == 0 || channel == message.getChannel()) &&
                           (noteSet.empty() || noteSet.find(message.getNoteNumber()) != noteSet.end());
                },
                [](MidiMessage message) { return message.getNoteNumber(); });
    };
}

std::function<void()> dsp::MidiInput::processNoteOn(int channel, std::unordered_set<int> noteSet) {
    return [this, channel, noteSet]() {
        processImpulse(
                [channel, noteSet](MidiMessage message) {
                    return message.isNoteOn() && (channel == 0 || channel == message.getChannel()) &&
                           (noteSet.empty() || noteSet.find(message.getNoteNumber()) != noteSet.end());
                },
                [](MidiMessage message) { return byteToUnipolar(message.getVelocity()); });
    };
}

std::function<void()> dsp::MidiInput::processNoteOff(int channel, std::unordered_set<int> noteSet) {
    return [this, channel, noteSet]() {
        processImpulse(
                [channel, noteSet](MidiMessage message) {
                    return message.isNoteOff() && (channel == 0 || channel == message.getChannel()) &&
                           (noteSet.empty() || noteSet.find(message.getNoteNumber()) != noteSet.end());
                },
                [](MidiMessage message) { return 1.0; });
    };
}

std::function<void()> dsp::MidiInput::processNotePressure(int channel, std::unordered_set<int> noteSet) {
    return [this, channel, noteSet]() {
        processContinuous(
                [channel, noteSet](MidiMessage message) {
                    return (message.isAftertouch() || message.isNoteOn() || message.isNoteOff()) &&
                           (channel == 0 || channel == message.getChannel()) &&
                           (noteSet.empty() || noteSet.find(message.getNoteNumber()) != noteSet.end());
                },
                [](MidiMessage message) {
                    return byteToUnipolar(message.isAftertouch() ? message.getAfterTouchValue()
                                                                 : message.isNoteOn() ? message.getVelocity() : 0);
                });
    };
}

std::function<void()> dsp::MidiInput::processControl(int channel, std::unordered_set<int> controlSet) {
    return [this, channel, controlSet]() {
        processContinuous(
                [channel, controlSet](MidiMessage message) {
                    return message.isController() && (channel == 0 || channel == message.getChannel()) &&
                           (controlSet.empty() || controlSet.find(message.getNoteNumber()) != controlSet.end());
                },
                [](MidiMessage message) { return message.getControllerNumber(); });
    };
}

std::function<void()> dsp::MidiInput::processControlValue(int channel, std::unordered_set<int> controlSet) {
    return [this, channel, controlSet]() {
        processContinuous(
                [channel, controlSet](MidiMessage message) {
                    return message.isController() && (channel == 0 || channel == message.getChannel()) &&
                           (controlSet.empty() || controlSet.find(message.getNoteNumber()) != controlSet.end());
                },
                [](MidiMessage message) { return byteToUnipolar(message.getControllerValue()); });
    };
}

std::function<void()> dsp::MidiInput::processProgram(int channel) {
    return [this, channel]() {
        processContinuous(
                [channel](MidiMessage message) {
                    return message.isProgramChange() && (channel == 0 || channel == message.getChannel());
                },
                [](MidiMessage message) { return message.getProgramChangeNumber(); });
    };
}

std::function<void()> dsp::MidiInput::processChannelPressure(int channel) {
    return [this, channel]() {
        processContinuous(
                [channel](MidiMessage message) {
                    return message.isChannelPressure() && (channel == 0 || channel == message.getChannel());
                },
                [](MidiMessage message) { return byteToUnipolar(message.getChannelPressureValue()); });
    };
}

std::function<void()> dsp::MidiInput::processPitchBend(int channel) {
    return [this, channel]() {
        processContinuous(
                [channel](MidiMessage message) {
                    return message.isPitchWheel() && (channel == 0 || channel == message.getChannel());
                },
                [](MidiMessage message) { return shortToBipolar(message.getPitchWheelValue()); });
    };
}

std::function<void()> dsp::MidiInput::processAllNotesOff(int channel) {
    return [this, channel]() {
        processImpulse(
                [channel](MidiMessage message) {
                    return message.isAllNotesOff() && (channel == 0 || channel == message.getChannel());
                },
                [](MidiMessage message) { return 1.0; });
    };
}

std::function<void()> dsp::MidiInput::processSongPositionInQuarterNotes() {
    return [this]() {
        processContinuous([](MidiMessage message) { return message.isSongPositionPointer(); },
                          [](MidiMessage message) { return message.getSongPositionPointerMidiBeat() * 0.25; });
    };
}

std::function<void()> dsp::MidiInput::processClock() {
    return [this]() {
        processImpulse([](MidiMessage message) { return message.isMidiClock(); },
                       [](MidiMessage message) { return 1.0; });
    };
}

std::function<void()> dsp::MidiInput::processStart() {
    return [this]() {
        processImpulse([](MidiMessage message) { return message.isMidiStart(); },
                       [](MidiMessage message) { return 1.0; });
    };
}

std::function<void()> dsp::MidiInput::processContinue() {
    return [this]() {
        processImpulse([](MidiMessage message) { return message.isMidiContinue(); },
                       [](MidiMessage message) { return 1.0; });
    };
}

std::function<void()> dsp::MidiInput::processStop() {
    return [this]() {
        processImpulse([](MidiMessage message) { return message.isMidiStop(); },
                       [](MidiMessage message) { return 1.0; });
    };
}

void dsp::MidiInput::processImpulse(std::function<bool(MidiMessage)> conditional,
                                    std::function<Sample(MidiMessage)> capture) {
    for (const auto meta : *inputMessages) {
        const MidiMessage message = meta.getMessage();
        const Sample current = conditional(message) ? capture(message) : 0.0;
        for (int channel = 0; channel < getNumOutputChannels(); ++channel) {
            getOutput()->getBlock().setSample(channel, meta.samplePosition, current);
        }
    }
}

void dsp::MidiInput::processContinuous(std::function<bool(MidiMessage)> conditional,
                                       std::function<Sample(MidiMessage)> capture) {
    int start = 0;
    for (const auto meta : *inputMessages) {
        const MidiMessage message = meta.getMessage();
        if (conditional(message)) {
            getOutput()->getBlock().getSubBlock(start, meta.samplePosition - start).fill(previous);
            start = meta.samplePosition;
            previous = capture(message);
        }
    }
    getOutput()->getBlock().getSubBlock(start, getOutput()->getNumSamples() - start).fill(previous);
}

void dsp::MidiInput::processNoLock() {
    if (processFunction) {
        processFunction();
    }
}
