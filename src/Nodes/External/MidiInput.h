#pragma once

#include "../Core/Producer.h"

#include <unordered_set>

namespace dsp {

class MidiInput : public Producer {

public:
    MidiInput(Type type, std::shared_ptr<MidiBuffer> inputMessages);

    std::shared_ptr<MidiBuffer> getInputMessages() const;

    std::function<void()> getProcessFunction() const;
    void setProcessFunction(std::function<void()> processFunction);

    std::function<void()> processNote(size_t channel, std::unordered_set<uint8> noteSet);
    std::function<void()> processNoteOn(size_t channel, std::unordered_set<uint8> noteSet);
    std::function<void()> processNoteOff(size_t channel, std::unordered_set<uint8> noteSet);
    std::function<void()> processNotePressure(size_t channel, std::unordered_set<uint8> noteSet);
    std::function<void()> processControl(size_t channel, std::unordered_set<uint8> controlSet);
    std::function<void()> processControlValue(size_t channel, std::unordered_set<uint8> controlSet);
    std::function<void()> processProgram(size_t channel);
    std::function<void()> processChannelPressure(size_t channel);
    std::function<void()> processPitchBend(size_t channel);
    std::function<void()> processAllNotesOff(size_t channel);
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
    std::shared_ptr<MidiBuffer> inputMessages;
    std::function<void()> processFunction;
    Sample previous;
};

} // namespace dsp
