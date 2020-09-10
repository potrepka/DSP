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

    std::function<void()> processNote(int channel, std::unordered_set<int> noteSet);
    std::function<void()> processNoteOn(int channel, std::unordered_set<int> noteSet);
    std::function<void()> processNoteOff(int channel, std::unordered_set<int> noteSet);
    std::function<void()> processNotePressure(int channel, std::unordered_set<int> noteSet);
    std::function<void()> processControl(int channel, std::unordered_set<int> controlSet);
    std::function<void()> processControlValue(int channel, std::unordered_set<int> controlSet);
    std::function<void()> processProgram(int channel);
    std::function<void()> processChannelPressure(int channel);
    std::function<void()> processPitchBend(int channel);
    std::function<void()> processAllNotesOff(int channel);
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
