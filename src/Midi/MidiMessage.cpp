#include "MidiMessage.h"

dsp::MidiMessage::MidiMessage(uint8_t byte0) {
    bytes.resize(1);
    bytes[0] = byte0;
}

dsp::MidiMessage::MidiMessage(uint8_t byte0, uint8_t byte1) {
    bytes.resize(2);
    bytes[0] = byte0;
    bytes[1] = byte1;
}

dsp::MidiMessage::MidiMessage(uint8_t byte0, uint8_t byte1, uint8_t byte2) {
    bytes.resize(3);
    bytes[0] = byte0;
    bytes[1] = byte1;
    bytes[2] = byte2;
}

dsp::MidiMessage::MidiMessage(const uint8_t *data, size_t size) {
    bytes.reserve(size);
    for (size_t i = 0; i < size; ++i) {
        bytes.push_back(data[i]);
    }
}

bool dsp::MidiMessage::isNote() const {
    return (bytes[0] & 0xf0) == 0x80 || (bytes[0] & 0xf0) == 0x90;
}

bool dsp::MidiMessage::isNoteOff() const {
    return (bytes[0] & 0xf0) == 0x80;
}

bool dsp::MidiMessage::isNoteOn() const {
    return (bytes[0] & 0xf0) == 0x90;
}

bool dsp::MidiMessage::isAftertouch() const {
    return (bytes[0] & 0xf0) == 0xa0;
}

bool dsp::MidiMessage::isController() const {
    return (bytes[0] & 0xf0) == 0xb0;
}

bool dsp::MidiMessage::isProgramChange() const {
    return (bytes[0] & 0xf0) == 0xc0;
}

bool dsp::MidiMessage::isChannelPressure() const {
    return (bytes[0] & 0xf0) == 0xd0;
}

bool dsp::MidiMessage::isPitchWheel() const {
    return (bytes[0] & 0xf0) == 0xe0;
}

bool dsp::MidiMessage::isAllNotesOff() const {
    return (bytes[0] & 0xf0) == 0xb0 && bytes[1] == 123;
}

bool dsp::MidiMessage::isSongPositionPointer() const {
    return bytes[0] == 0xf2;
}

bool dsp::MidiMessage::isMidiClock() const {
    return bytes[0] == 0xf8;
}

bool dsp::MidiMessage::isMidiStart() const {
    return bytes[0] == 0xfa;
}

bool dsp::MidiMessage::isMidiContinue() const {
    return bytes[0] == 0xfb;
}

bool dsp::MidiMessage::isMidiStop() const {
    return bytes[0] == 0xfc;
}

uint8_t dsp::MidiMessage::getChannel() const {
    return (bytes[0] & 0xf0) < 0xf0 ? (bytes[0] & 0xf) + 1 : 0;
}

uint8_t dsp::MidiMessage::getNoteNumber() const {
    DSP_ASSERT(isNote() || isAftertouch());
    return bytes[1];
}

uint8_t dsp::MidiMessage::getVelocity() const {
    DSP_ASSERT(isNote());
    return bytes[2];
}

uint8_t dsp::MidiMessage::getAfterTouchValue() const {
    DSP_ASSERT(isAftertouch());
    return bytes[2];
}

uint8_t dsp::MidiMessage::getControllerNumber() const {
    DSP_ASSERT(isController());
    return bytes[1];
}

uint8_t dsp::MidiMessage::getControllerValue() const {
    DSP_ASSERT(isController());
    return bytes[2];
}

uint8_t dsp::MidiMessage::getProgramChangeNumber() const {
    DSP_ASSERT(isProgramChange());
    return bytes[1];
}

uint8_t dsp::MidiMessage::getChannelPressureValue() const {
    DSP_ASSERT(isChannelPressure());
    return bytes[1];
}

int dsp::MidiMessage::getPitchWheelValue() const {
    DSP_ASSERT(isPitchWheel());
    return bytes[1] | (bytes[2] << 7);
}

int dsp::MidiMessage::getSongPositionPointerMidiBeat() const {
    DSP_ASSERT(isSongPositionPointer());
    return bytes[1] | (bytes[2] << 7);
}

dsp::MidiMessage dsp::MidiMessage::noteOff(uint8_t channel, uint8_t noteNumber, uint8_t velocity) {
    DSP_ASSERT(channel >= 1 && channel <= 16);
    DSP_ASSERT(noteNumber < 128);
    DSP_ASSERT(velocity < 128);
    return MidiMessage(0x80 | (channel - 1), noteNumber, velocity);
}

dsp::MidiMessage dsp::MidiMessage::noteOn(uint8_t channel, uint8_t noteNumber, uint8_t velocity) {
    DSP_ASSERT(channel >= 1 && channel <= 16);
    DSP_ASSERT(noteNumber < 128);
    DSP_ASSERT(velocity < 128);
    return MidiMessage(0x90 | (channel - 1), noteNumber, velocity);
}

dsp::MidiMessage dsp::MidiMessage::aftertouchChange(uint8_t channel, uint8_t noteNumber, uint8_t aftertouchValue) {
    DSP_ASSERT(channel >= 1 && channel <= 16);
    DSP_ASSERT(noteNumber < 128);
    DSP_ASSERT(aftertouchValue < 128);
    return MidiMessage(0xa0 | (channel - 1), noteNumber, aftertouchValue);
}

dsp::MidiMessage dsp::MidiMessage::controllerEvent(uint8_t channel, uint8_t controllerNumber, uint8_t controllerValue) {
    DSP_ASSERT(channel >= 1 && channel <= 16);
    DSP_ASSERT(controllerNumber < 128);
    DSP_ASSERT(controllerValue < 128);
    return MidiMessage(0xb0 | (channel - 1), controllerNumber, controllerValue);
}

dsp::MidiMessage dsp::MidiMessage::programChange(uint8_t channel, uint8_t programNumber) {
    DSP_ASSERT(channel >= 1 && channel <= 16);
    DSP_ASSERT(programNumber < 128);
    return MidiMessage(0xc0 | (channel - 1), programNumber);
}

dsp::MidiMessage dsp::MidiMessage::channelPressureChange(uint8_t channel, uint8_t channelPressureValue) {
    DSP_ASSERT(channel >= 1 && channel <= 16);
    DSP_ASSERT(channelPressureValue < 128);
    return MidiMessage(0xd0 | (channel - 1), channelPressureValue);
}

dsp::MidiMessage dsp::MidiMessage::pitchWheel(uint8_t channel, int pitchWheelValue) {
    DSP_ASSERT(channel >= 1 && channel <= 16);
    return MidiMessage(0xe0 | (channel - 1), pitchWheelValue & 127, (pitchWheelValue >> 7) & 127);
}

dsp::MidiMessage dsp::MidiMessage::allNotesOff(uint8_t channel) {
    return controllerEvent(channel, 123, 0);
}

dsp::MidiMessage dsp::MidiMessage::songPositionPointer(int songPositionPointerMidiBeat) {
    return MidiMessage(0xf2, songPositionPointerMidiBeat & 127, (songPositionPointerMidiBeat >> 7) & 127);
}

dsp::MidiMessage dsp::MidiMessage::midiClock() {
    return MidiMessage(0xf8);
}

dsp::MidiMessage dsp::MidiMessage::midiStart() {
    return MidiMessage(0xfa);
}

dsp::MidiMessage dsp::MidiMessage::midiContinue() {
    return MidiMessage(0xfb);
}

dsp::MidiMessage dsp::MidiMessage::midiStop() {
    return MidiMessage(0xfc);
}

std::vector<uint8_t> &dsp::MidiMessage::getBytes() {
    return bytes;
}

const uint8_t *dsp::MidiMessage::getRawData() const {
    return bytes.data();
}

size_t dsp::MidiMessage::getRawDataSize() const {
    return bytes.size();
}

dsp::TimedMidiMessage::TimedMidiMessage(MidiMessage midiMessage, size_t samplePosition)
        : samplePosition(samplePosition)
        , midiMessage(midiMessage) {}

dsp::MidiMessage dsp::TimedMidiMessage::getMessage() const {
    return midiMessage;
}
