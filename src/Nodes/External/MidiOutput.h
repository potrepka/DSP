#pragma once

#include "../Core/Consumer.h"

#include <unordered_set>

namespace dsp {

class MidiOutput : public Consumer {

public:
    MidiOutput(Type type, std::shared_ptr<MidiData> outputMessages);

    std::shared_ptr<MidiData> getOutputMessages() const;

    std::function<void()> getProcessFunction() const;
    void setProcessFunction(std::function<void()> processFunction);

    std::function<void()> processNote(int channel);
    std::function<void()> processNoteOn(int channel, std::unordered_set<int> noteSet);
    std::function<void()> processNoteOff(int channel, std::unordered_set<int> noteSet);
    std::function<void()> processNotePressure(int channel, std::unordered_set<int> noteSet);
    std::function<void()> processControl(int channel);
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
    void processImpulse(std::unordered_set<int> itemSet, std::function<MidiMessage(int, Sample)> publish);
    void processImpulse(std::unordered_set<int> itemSet, std::function<MidiMessage(int)> publish);
    void processImpulse(std::function<MidiMessage(Sample)> publish);
    void processImpulse(std::function<MidiMessage()> publish);
    void processContinuous(std::unordered_set<int> itemSet, std::function<MidiMessage(int, Sample)> publish);
    void processContinuous(std::function<MidiMessage(Sample)> publish);
    void processNoLock();

private:
    std::shared_ptr<MidiData> outputMessages;
    std::function<void()> processFunction;
    Sample previous;
};

} // namespace dsp
