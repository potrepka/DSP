#pragma once

#include "../Core/Consumer.h"

#include <unordered_set>

namespace dsp {

class MidiOutput : public Consumer {

public:
    MidiOutput(Type type, std::shared_ptr<MidiBuffer> outputMessages);

    std::shared_ptr<MidiBuffer> getOutputMessages() const;

    std::function<void()> getProcessFunction() const;
    void setProcessFunction(std::function<void()> processFunction);

    std::function<void()> processNote(size_t channel);
    std::function<void()> processNoteOn(size_t channel, std::unordered_set<uint8> noteSet);
    std::function<void()> processNoteOff(size_t channel, std::unordered_set<uint8> noteSet);
    std::function<void()> processNotePressure(size_t channel, std::unordered_set<uint8> noteSet);
    std::function<void()> processControl(size_t channel);
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
    void processImpulse(std::unordered_set<uint8> itemSet, std::function<MidiMessage(uint8, Sample)> publish);
    void processImpulse(std::unordered_set<uint8> itemSet, std::function<MidiMessage(uint8)> publish);
    void processImpulse(std::function<MidiMessage(Sample)> publish);
    void processImpulse(std::function<MidiMessage()> publish);
    void processContinuous(std::unordered_set<uint8> itemSet, std::function<MidiMessage(uint8, Sample)> publish);
    void processContinuous(std::function<MidiMessage(Sample)> publish);
    void processNoLock();

private:
    std::shared_ptr<MidiBuffer> outputMessages;
    std::function<void()> processFunction;
    Sample previous;
};

} // namespace dsp
