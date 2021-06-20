#include "MidiInput.h"

dsp::MidiInput::MidiInput(std::shared_ptr<MidiBuffer> inputMessages, Type type)
        : Producer(type)
        , inputMessages(inputMessages)
        , previous(0.0) {}

std::shared_ptr<dsp::MidiBuffer> dsp::MidiInput::getInputMessages() const {
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

std::function<void()> dsp::MidiInput::processNote(uint8 channel, std::unordered_set<uint8> noteSet) {
    return [this, channel, noteSet]() {
        processContinuous(
                [channel, noteSet](MidiMessage message) {
                    return message.isNoteOn() && (channel == 0 || channel == message.getChannel()) &&
                           (noteSet.empty() || noteSet.find(message.getNoteNumber()) != noteSet.end());
                },
                [](MidiMessage message) { return message.getNoteNumber(); });
    };
}

std::function<void()> dsp::MidiInput::processNoteOn(uint8 channel, std::unordered_set<uint8> noteSet) {
    return [this, channel, noteSet]() {
        processImpulse(
                [channel, noteSet](MidiMessage message) {
                    return message.isNoteOn() && (channel == 0 || channel == message.getChannel()) &&
                           (noteSet.empty() || noteSet.find(message.getNoteNumber()) != noteSet.end());
                },
                [](MidiMessage message) { return byteToUnipolar(message.getVelocity()); });
    };
}

std::function<void()> dsp::MidiInput::processNoteOff(uint8 channel, std::unordered_set<uint8> noteSet) {
    return [this, channel, noteSet]() {
        processImpulse(
                [channel, noteSet](MidiMessage message) {
                    return message.isNoteOff() && (channel == 0 || channel == message.getChannel()) &&
                           (noteSet.empty() || noteSet.find(message.getNoteNumber()) != noteSet.end());
                },
                [](MidiMessage message) { return 1.0; });
    };
}

std::function<void()> dsp::MidiInput::processNotePressure(uint8 channel, std::unordered_set<uint8> noteSet) {
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

std::function<void()> dsp::MidiInput::processControl(uint8 channel, std::unordered_set<uint8> controlSet) {
    return [this, channel, controlSet]() {
        processContinuous(
                [channel, controlSet](MidiMessage message) {
                    return message.isController() && (channel == 0 || channel == message.getChannel()) &&
                           (controlSet.empty() || controlSet.find(message.getNoteNumber()) != controlSet.end());
                },
                [](MidiMessage message) { return message.getControllerNumber(); });
    };
}

std::function<void()> dsp::MidiInput::processControlValue(uint8 channel, std::unordered_set<uint8> controlSet) {
    return [this, channel, controlSet]() {
        processContinuous(
                [channel, controlSet](MidiMessage message) {
                    return message.isController() && (channel == 0 || channel == message.getChannel()) &&
                           (controlSet.empty() || controlSet.find(message.getNoteNumber()) != controlSet.end());
                },
                [](MidiMessage message) { return byteToUnipolar(message.getControllerValue()); });
    };
}

std::function<void()> dsp::MidiInput::processProgram(uint8 channel) {
    return [this, channel]() {
        processContinuous(
                [channel](MidiMessage message) {
                    return message.isProgramChange() && (channel == 0 || channel == message.getChannel());
                },
                [](MidiMessage message) { return message.getProgramChangeNumber(); });
    };
}

std::function<void()> dsp::MidiInput::processChannelPressure(uint8 channel) {
    return [this, channel]() {
        processContinuous(
                [channel](MidiMessage message) {
                    return message.isChannelPressure() && (channel == 0 || channel == message.getChannel());
                },
                [](MidiMessage message) { return byteToUnipolar(message.getChannelPressureValue()); });
    };
}

std::function<void()> dsp::MidiInput::processPitchBend(uint8 channel) {
    return [this, channel]() {
        processContinuous(
                [channel](MidiMessage message) {
                    return message.isPitchWheel() && (channel == 0 || channel == message.getChannel());
                },
                [](MidiMessage message) { return shortToBipolar(message.getPitchWheelValue()); });
    };
}

std::function<void()> dsp::MidiInput::processAllNotesOff(uint8 channel) {
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
        for (size_t channel = 0; channel < getNumOutputChannels(); ++channel) {
            getOutput()->getWrapper().setSample(channel, meta.samplePosition, current);
        }
    }
}

void dsp::MidiInput::processContinuous(std::function<bool(MidiMessage)> conditional,
                                       std::function<Sample(MidiMessage)> capture) {
    size_t startSample = 0;
    for (const auto meta : *inputMessages) {
        const MidiMessage message = meta.getMessage();
        if (conditional(message)) {
            getOutput()->getWrapper().getSampleRange(startSample, meta.samplePosition - startSample).fill(previous);
            startSample = meta.samplePosition;
            previous = capture(message);
        }
    }
    getOutput()->getWrapper().getSampleRange(startSample, getOutput()->getNumSamples() - startSample).fill(previous);
}

void dsp::MidiInput::processNoLock() {
    if (processFunction) {
        processFunction();
    }
}
