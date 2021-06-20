#pragma once

#include "../Core/Producer.h"

#include <unordered_set>

namespace dsp {

class MidiInput : public Producer {

public:
    MidiInput(std::shared_ptr<MidiBuffer> inputMessages, Type type = Type::RATIO);

    std::shared_ptr<MidiBuffer> getInputMessages() const;

    std::function<void()> getProcessFunction() const;
    void setProcessFunction(std::function<void()> processFunction);

    std::function<void()> processNote(uint8 channel, std::unordered_set<uint8> noteSet);
    std::function<void()> processNoteOn(uint8 channel, std::unordered_set<uint8> noteSet);
    std::function<void()> processNoteOff(uint8 channel, std::unordered_set<uint8> noteSet);
    std::function<void()> processNotePressure(uint8 channel, std::unordered_set<uint8> noteSet);
    std::function<void()> processControl(uint8 channel, std::unordered_set<uint8> controlSet);
    std::function<void()> processControlValue(uint8 channel, std::unordered_set<uint8> controlSet);
    std::function<void()> processProgram(uint8 channel);
    std::function<void()> processChannelPressure(uint8 channel);
    std::function<void()> processPitchBend(uint8 channel);
    std::function<void()> processAllNotesOff(uint8 channel);
    std::function<void()> processSongPositionInQuarterNotes();
    std::function<void()> processClock();
    std::function<void()> processStart();
    std::function<void()> processContinue();
    std::function<void()> processStop();

protected:
    void processImpulse(std::function<bool(MidiMessage)> conditional, std::function<Sample(MidiMessage)> capture);
    void processContinuous(std::function<bool(MidiMessage)> conditional, std::function<Sample(MidiMessage)> capture);
    void processNoLock();

private:
    const std::shared_ptr<MidiBuffer> inputMessages;
    std::function<void()> processFunction;
    Sample previous;
};

} // namespace dsp
