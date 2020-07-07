#include "Midi.h"

const unsigned char MIDI_CHANNELS = 16;
const unsigned char MIDI_NOTES = 128;

std::string dsp::Midi::TimedMessage::getMessageTypeName(MessageType type) {
    switch (type) {
        case MIDI_UNKNOWN: return "Unknown";
        case MIDI_NOTE_OFF: return "Note Off";
        case MIDI_NOTE_ON: return "Note On";
        case MIDI_POLY_AFTERTOUCH: return "Note Pressure";
        case MIDI_CONTROL_CHANGE: return "Control Change";
        case MIDI_PROGRAM_CHANGE: return "Program Change";
        case MIDI_AFTERTOUCH: return "Channel Pressure";
        case MIDI_PITCH_BEND: return "Pitch Bend";
        case MIDI_SYSEX: return "System Exclusive";
        case MIDI_TIME_CODE: return "Time Code";
        case MIDI_SONG_POS_POINTER: return "Song Position Pointer";
        case MIDI_SONG_SELECT: return "Song Select";
        case MIDI_TUNE_REQUEST: return "Tune Request";
        case MIDI_SYSEX_END: return "System Exclusive End";
        case MIDI_TIME_CLOCK: return "Time Clock";
        case MIDI_START: return "Start";
        case MIDI_CONTINUE: return "Continue";
        case MIDI_STOP: return "Stop";
        case MIDI_ACTIVE_SENSING: return "Active Sensing";
        case MIDI_SYSTEM_RESET: return "System Reset";
    }
}

dsp::Midi::TimedMessage::TimedMessage(double time, std::vector<unsigned char> bytes) {
    this->time = time;
    this->bytes = bytes;
}

double dsp::Midi::TimedMessage::getTime() const {
    return time;
}

std::vector<unsigned char> dsp::Midi::TimedMessage::getBytes() const {
    return bytes;
}

dsp::Midi::TimedMessage::MessageType dsp::Midi::TimedMessage::getMessageType() const {
    if (bytes[0] >= MIDI_SYSEX) {
        return (MessageType)bytes[0];
    } else {
        return (MessageType)(bytes[0] & 0xF0);
    }
}

unsigned char dsp::Midi::TimedMessage::getChannel() const {
    if (bytes[0] >= MIDI_SYSEX) {
        return 0;
    } else {
        return (unsigned int)(bytes[0] & 0x0F) + 1;
    }
}

unsigned char dsp::Midi::TimedMessage::getByte(unsigned int index) const {
    return bytes[index];
}

DSP_FLOAT dsp::Midi::TimedMessage::getByteAsUnipolar(unsigned int index) const {
    return bytes[index] == 0 ? 0.0 : (bytes[index] + 1) * 0.0078125;
}

void dsp::Midi::TimedMessage::setByte(unsigned int index, unsigned char value) {
    bytes[index] = value;
}

void dsp::Midi::TimedMessage::setByteUsingUnipolar(unsigned int index, DSP_FLOAT value) {
    bytes[index] = value == 0.0 ? 0 : static_cast<unsigned char>(value * 128 - 1);
}

unsigned short dsp::Midi::TimedMessage::getShort(unsigned int lsb, unsigned int msb) const {
    return bytes[lsb] + bytes[msb] * 128;
}

DSP_FLOAT dsp::Midi::TimedMessage::getShortAsBipolar(unsigned int lsb, unsigned int msb) const {
    unsigned short x = getShort(lsb, msb) - 8192;
    return (x > 0 ? x + 1 : x) * 0.0001220703125;
}

void dsp::Midi::TimedMessage::setShort(unsigned int lsb, unsigned int msb, unsigned short value) {
    unsigned char least = value % 128;
    unsigned char most = value / 128;
    bytes[lsb] = least;
    bytes[msb] = most;
}

void dsp::Midi::TimedMessage::setShortUsingBipolar(unsigned int lsb, unsigned int msb, DSP_FLOAT bipolar) {
    setShort(lsb, msb, static_cast<unsigned short>(bipolar * 8192 + 8192));
}

void dsp::Midi::MidiInput::callback(double delta, std::vector<unsigned char> *bytesPointer, void *data) {
    MidiInput *input = (MidiInput *)data;
    input->pushQueue(delta, *bytesPointer);
}

dsp::Midi::MidiInput::MidiInput(unsigned int port) : messageTime(0.0), bufferSamples(0) {
#if USE_RTMIDI
    midiIn.setCallback(MidiInput::callback, this);
#endif
    setPort(port);

    notePressureState.resize(MIDI_CHANNELS);
    controlChangeState.resize(MIDI_CHANNELS);
    programChangeState.resize(MIDI_CHANNELS);
    channelPressureState.resize(MIDI_CHANNELS);
    pitchBendState.resize(MIDI_CHANNELS);

    noteOn.resize(MIDI_CHANNELS);
    noteOff.resize(MIDI_CHANNELS);
    notePressure.resize(MIDI_CHANNELS);
    controlChange.resize(MIDI_CHANNELS);
    programChange.resize(MIDI_CHANNELS);
    channelPressure.resize(MIDI_CHANNELS);
    pitchBend.resize(MIDI_CHANNELS);

    play = std::make_shared<Input>(getBufferSize(), Type::BINARY, Space::TIME, 0.0);
    stop = std::make_shared<Input>(getBufferSize(), Type::BINARY, Space::TIME, 0.0);
    reset = std::make_shared<Input>(getBufferSize(), Type::BINARY, Space::TIME, 0.0);
    clock = std::make_shared<Input>(getBufferSize(), Type::BINARY, Space::TIME, 0.0);

    for (unsigned char i = 0; i < MIDI_CHANNELS; i++) {
        notePressureState[i].resize(MIDI_NOTES);
        controlChangeState[i].resize(MIDI_NOTES);
        programChangeState[i] = 0.0;
        channelPressureState[i] = 0.0;
        pitchBendState[i] = 0.0;

        noteOn[i].resize(MIDI_NOTES);
        noteOff[i].resize(MIDI_NOTES);
        notePressure[i].resize(MIDI_NOTES);
        controlChange[i].resize(MIDI_NOTES);
        programChange[i] = std::make_shared<Input>(getBufferSize(), Type::INTEGER, Space::TIME, 0.0);
        channelPressure[i] = std::make_shared<Input>(getBufferSize(), Type::UNIPOLAR, Space::TIME, 0.0);
        pitchBend[i] = std::make_shared<Input>(getBufferSize(), Type::BIPOLAR, Space::TIME, 0.0);

        for (unsigned char n = 0; n < MIDI_NOTES; n++) {
            notePressureState[i][n] = 0.0;
            controlChangeState[i][n] = 0.0;

            noteOn[i][n] = std::make_shared<Input>(getBufferSize(), Type::UNIPOLAR, Space::TIME, 0.0);
            noteOff[i][n] = std::make_shared<Input>(getBufferSize(), Type::UNIPOLAR, Space::TIME, 0.0);
            notePressure[i][n] = std::make_shared<Input>(getBufferSize(), Type::UNIPOLAR, Space::TIME, 0.0);
            controlChange[i][n] = std::make_shared<Input>(getBufferSize(), Type::UNIPOLAR, Space::TIME, 0.0);
        }
    }
}

std::string dsp::Midi::MidiInput::getName() const {
    return name;
}

void dsp::Midi::MidiInput::setPort(unsigned int port) {
#if USE_RTMIDI
    midiIn.openPort(port);
    name = midiIn.getPortName(port);
#endif
}

std::shared_ptr<dsp::Input> dsp::Midi::MidiInput::getNoteOnTrigger(unsigned char channel, unsigned char note) {
    return noteOn[channel][note];
}

std::shared_ptr<dsp::Input> dsp::Midi::MidiInput::getNoteOffTrigger(unsigned char channel, unsigned char note) {
    return noteOff[channel][note];
}

std::shared_ptr<dsp::Input> dsp::Midi::MidiInput::getNotePressure(unsigned char channel, unsigned char note) {
    return notePressure[channel][note];
}

std::shared_ptr<dsp::Input> dsp::Midi::MidiInput::getControlChange(unsigned char channel, unsigned char control) {
    return controlChange[channel][control];
}

std::shared_ptr<dsp::Input> dsp::Midi::MidiInput::getProgramChange(unsigned char channel) {
    return programChange[channel];
}

std::shared_ptr<dsp::Input> dsp::Midi::MidiInput::getChannelPressure(unsigned char channel) {
    return channelPressure[channel];
}

std::shared_ptr<dsp::Input> dsp::Midi::MidiInput::getPitchBend(unsigned char channel) {
    return pitchBend[channel];
}

std::shared_ptr<dsp::Input> dsp::Midi::MidiInput::getPlayTrigger() {
    return play;
}

std::shared_ptr<dsp::Input> dsp::Midi::MidiInput::getStopTrigger() {
    return stop;
}

std::shared_ptr<dsp::Input> dsp::Midi::MidiInput::getResetTrigger() {
    return reset;
}

std::shared_ptr<dsp::Input> dsp::Midi::MidiInput::getClockTrigger() {
    return clock;
}

unsigned int dsp::Midi::MidiInput::getNumCallbacks() {
    return static_cast<unsigned int>(callbacks.size());
}

std::function<void(dsp::Midi::TimedMessage)> dsp::Midi::MidiInput::getCallback(unsigned int index) {
    return *callbacks[index];
}

void dsp::Midi::MidiInput::pushCallback(std::function<void(TimedMessage)> callback) {
    callbacks.push_back(std::make_shared<std::function<void(TimedMessage)>>(callback));
}

void dsp::Midi::MidiInput::removeCallback(unsigned int index) {
    callbacks.erase(callbacks.begin() + index);
}

void dsp::Midi::MidiInput::run() {
    lock();
    for (unsigned int k = 0; k < getBufferSize(); k++) {
        bool noteOnState = false;
        bool noteOffState = false;
        bool playState = false;
        bool stopState = false;
        bool resetState = false;
        bool clockState = false;
        if (queue.top().getTime() <= bufferSamples * getOneOverSampleRate()) {
            const TimedMessage &message = queue.top();
            const TimedMessage::MessageType type = message.getMessageType();
            const unsigned char channel = message.getChannel();
            switch (type) {
                case TimedMessage::MIDI_NOTE_OFF: noteOffState = true; break;
                case TimedMessage::MIDI_NOTE_ON:
                    noteOnState = true;
                    notePressureState[channel][message.getByte(1)] = message.getByteAsUnipolar(2);
                    break;
                case TimedMessage::MIDI_POLY_AFTERTOUCH:
                    notePressureState[channel][message.getByte(1)] = message.getByteAsUnipolar(2);
                    break;
                case TimedMessage::MIDI_CONTROL_CHANGE:
                    controlChangeState[channel][message.getByte(1)] = message.getByteAsUnipolar(2);
                    break;
                case TimedMessage::MIDI_PROGRAM_CHANGE: programChangeState[channel] = message.getByte(1); break;
                case TimedMessage::MIDI_AFTERTOUCH: channelPressureState[channel] = message.getByteAsUnipolar(1); break;
                case TimedMessage::MIDI_PITCH_BEND: pitchBendState[channel] = message.getShortAsBipolar(1, 2); break;

                case TimedMessage::MIDI_TIME_CODE:
                case TimedMessage::MIDI_SONG_POS_POINTER:
                case TimedMessage::MIDI_SONG_SELECT:
                case TimedMessage::MIDI_TUNE_REQUEST: break;

                case TimedMessage::MIDI_TIME_CLOCK: clockState = true; break;
                case TimedMessage::MIDI_START:
                    playState = true;
                    resetState = true;
                    break;
                case TimedMessage::MIDI_CONTINUE: playState = true; break;
                case TimedMessage::MIDI_STOP: stopState = true; break;

                case TimedMessage::MIDI_UNKNOWN:
                case TimedMessage::MIDI_SYSEX:
                case TimedMessage::MIDI_SYSEX_END:
                case TimedMessage::MIDI_ACTIVE_SENSING:
                case TimedMessage::MIDI_SYSTEM_RESET: break;
            }
        }
        for (unsigned char i = 0; i < MIDI_CHANNELS; i++) {
            for (unsigned char n = 0; n < MIDI_NOTES; n++) {
                noteOn[i][n]->getBuffer()[k] = noteOnState ? 1.0 : 0.0;
                noteOff[i][n]->getBuffer()[k] = noteOffState ? 1.0 : 0.0;
                notePressure[i][n]->getBuffer()[k] = notePressureState[i][n];
                controlChange[i][n]->getBuffer()[k] = controlChangeState[i][n];
            }
            programChange[i]->getBuffer()[k] = programChangeState[i];
            channelPressure[i]->getBuffer()[k] = channelPressureState[i];
            pitchBend[i]->getBuffer()[k] = pitchBendState[i];
        }
        play->getBuffer()[k] = playState ? 1.0 : 0.0;
        stop->getBuffer()[k] = stopState ? 1.0 : 0.0;
        reset->getBuffer()[k] = resetState ? 1.0 : 0.0;
        clock->getBuffer()[k] = clockState ? 1.0 : 0.0;
        bufferSamples++;
    }
    unlock();
}

void dsp::Midi::MidiInput::pushQueue(double delta, std::vector<unsigned char> bytes) {
    messageTime += delta;
    TimedMessage message(messageTime, bytes);
    queue.push(message);
    runCallbacks(message);
}

void dsp::Midi::MidiInput::runCallbacks(TimedMessage message) {
    for (const auto &callback : callbacks) {
        (*callback)(message);
    }
}

dsp::Midi::MidiOutput::MidiOutput(unsigned int port) {
    setPort(port);

    notePressureState.resize(MIDI_CHANNELS);
    controlChangeState.resize(MIDI_CHANNELS);
    programChangeState.resize(MIDI_CHANNELS);
    channelPressureState.resize(MIDI_CHANNELS);
    pitchBendState.resize(MIDI_CHANNELS);

    noteOn.resize(MIDI_CHANNELS);
    noteOff.resize(MIDI_CHANNELS);
    notePressure.resize(MIDI_CHANNELS);
    controlChange.resize(MIDI_CHANNELS);
    programChange.resize(MIDI_CHANNELS);
    channelPressure.resize(MIDI_CHANNELS);
    pitchBend.resize(MIDI_CHANNELS);

    play = std::make_shared<Output>(getBufferSize(), Type::BINARY, Space::TIME, 0.0);
    stop = std::make_shared<Output>(getBufferSize(), Type::BINARY, Space::TIME, 0.0);
    reset = std::make_shared<Output>(getBufferSize(), Type::BINARY, Space::TIME, 0.0);
    clock = std::make_shared<Output>(getBufferSize(), Type::BINARY, Space::TIME, 0.0);

    for (unsigned char i = 0; i < MIDI_CHANNELS; i++) {
        notePressureState[i].resize(MIDI_NOTES);
        controlChangeState[i].resize(MIDI_NOTES);
        programChangeState[i] = 0;
        channelPressureState[i] = 0;
        pitchBendState[i] = 0;

        noteOn[i].resize(MIDI_NOTES);
        noteOff[i].resize(MIDI_NOTES);
        notePressure[i].resize(MIDI_NOTES);
        controlChange[i].resize(MIDI_NOTES);
        programChange[i] = std::make_shared<Output>(getBufferSize(), Type::INTEGER, Space::TIME, 0.0);
        channelPressure[i] = std::make_shared<Output>(getBufferSize(), Type::UNIPOLAR, Space::TIME, 0.0);
        pitchBend[i] = std::make_shared<Output>(getBufferSize(), Type::BIPOLAR, Space::TIME, 0.0);

        for (unsigned char n = 0; n < MIDI_NOTES; n++) {
            notePressureState[i][n] = 0;
            controlChangeState[i][n] = 0;

            noteOn[i][n] = std::make_shared<Output>(getBufferSize(), Type::UNIPOLAR, Space::TIME, 0.0);
            noteOff[i][n] = std::make_shared<Output>(getBufferSize(), Type::UNIPOLAR, Space::TIME, 0.0);
            notePressure[i][n] = std::make_shared<Output>(getBufferSize(), Type::UNIPOLAR, Space::TIME, 0.0);
            controlChange[i][n] = std::make_shared<Output>(getBufferSize(), Type::UNIPOLAR, Space::TIME, 0.0);
        }
    }
}

std::string dsp::Midi::MidiOutput::getName() const {
    return name;
}

void dsp::Midi::MidiOutput::setPort(unsigned int port) {
#if USE_RTMIDI
    midiOut.openPort(port);
    name = midiOut.getPortName(port);
#endif
}

std::shared_ptr<dsp::Output> dsp::Midi::MidiOutput::getNoteOnTrigger(unsigned char channel, unsigned char note) {
    return noteOn[channel][note];
}

std::shared_ptr<dsp::Output> dsp::Midi::MidiOutput::getNoteOffTrigger(unsigned char channel, unsigned char note) {
    return noteOff[channel][note];
}

std::shared_ptr<dsp::Output> dsp::Midi::MidiOutput::getNotePressure(unsigned char channel, unsigned char note) {
    return notePressure[channel][note];
}

std::shared_ptr<dsp::Output> dsp::Midi::MidiOutput::getControlChange(unsigned char channel, unsigned char control) {
    return controlChange[channel][control];
}

std::shared_ptr<dsp::Output> dsp::Midi::MidiOutput::getProgramChange(unsigned char channel) {
    return programChange[channel];
}

std::shared_ptr<dsp::Output> dsp::Midi::MidiOutput::getChannelPressure(unsigned char channel) {
    return channelPressure[channel];
}

std::shared_ptr<dsp::Output> dsp::Midi::MidiOutput::getPitchBend(unsigned char channel) {
    return pitchBend[channel];
}

std::shared_ptr<dsp::Output> dsp::Midi::MidiOutput::getPlayTrigger() {
    return play;
}

std::shared_ptr<dsp::Output> dsp::Midi::MidiOutput::getStopTrigger() {
    return stop;
}

std::shared_ptr<dsp::Output> dsp::Midi::MidiOutput::getResetTrigger() {
    return reset;
}

std::shared_ptr<dsp::Output> dsp::Midi::MidiOutput::getClockTrigger() {
    return clock;
}

void dsp::Midi::MidiOutput::run() {
    lock();
    TimedMessage message1(0.0, std::vector<unsigned char>(1));
    TimedMessage message2(0.0, std::vector<unsigned char>(2));
    TimedMessage message3(0.0, std::vector<unsigned char>(3));
    for (unsigned int k = 0; k < getBufferSize(); k++) {
        int64_t nanoseconds = static_cast<int64_t>(1000000000 * getOneOverSampleRate() * k);
        for (unsigned char i = 0; i < MIDI_CHANNELS; i++) {
            for (unsigned char n = 0; n < MIDI_NOTES; n++) {
                if (noteOff[i][n]->getBuffer()[k]) {
                    message3.setByte(0, TimedMessage::MessageType::MIDI_NOTE_OFF);
                    message3.setByte(1, n);
                    message3.setByte(2, 0);
                    sendMessageWithDelay(nanoseconds, message3.getBytes());
                }
                if (noteOn[i][n]->getBuffer()[k]) {
                    message3.setByte(0, TimedMessage::MessageType::MIDI_NOTE_ON);
                    message3.setByte(1, n);
                    message3.setByteUsingUnipolar(2, notePressure[i][n]->getBuffer()[k]);
                    sendMessageWithDelay(nanoseconds, message3.getBytes());
                } else {
                    message3.setByteUsingUnipolar(2, notePressure[i][n]->getBuffer()[k]);
                    if (notePressureState[i][n] != message3.getByte(2)) {
                        notePressureState[i][n] = message3.getByte(2);
                        message3.setByte(0, TimedMessage::MessageType::MIDI_POLY_AFTERTOUCH + i);
                        message3.setByte(1, n);
                        sendMessageWithDelay(nanoseconds, message3.getBytes());
                    }
                }
                message3.setByteUsingUnipolar(2, controlChange[i][n]->getBuffer()[k]);
                if (controlChangeState[i][n] != message3.getByte(2)) {
                    controlChangeState[i][n] = message3.getByte(2);
                    message3.setByte(0, TimedMessage::MessageType::MIDI_CONTROL_CHANGE + i);
                    message3.setByte(1, n);
                    sendMessageWithDelay(nanoseconds, message3.getBytes());
                }
            }
            message2.setByte(1, programChange[i]->getBuffer()[k]);
            if (programChangeState[i] != message2.getByte(1)) {
                programChangeState[i] = message2.getByte(1);
                message2.setByte(0, TimedMessage::MessageType::MIDI_PROGRAM_CHANGE + i);
                sendMessageWithDelay(nanoseconds, message2.getBytes());
            }
            message2.setByteUsingUnipolar(1, channelPressure[i]->getBuffer()[k]);
            if (channelPressureState[i] != message2.getByte(1)) {
                channelPressureState[i] = message2.getByte(1);
                message2.setByte(0, TimedMessage::MessageType::MIDI_AFTERTOUCH + i);
                sendMessageWithDelay(nanoseconds, message2.getBytes());
            }
            message3.setShortUsingBipolar(1, 2, pitchBend[i]->getBuffer()[k]);
            if (pitchBendState[i] != message3.getShort(1, 2)) {
                pitchBendState[i] = message3.getShort(1, 2);
                message3.setByte(0, TimedMessage::MessageType::MIDI_PITCH_BEND + i);
                sendMessageWithDelay(nanoseconds, message3.getBytes());
            }
            if (play->getBuffer()[k]) {
                message1.setByte(0,
                                 reset->getBuffer()[k] ? TimedMessage::MessageType::MIDI_START
                                                       : TimedMessage::MessageType::MIDI_CONTINUE);
                sendMessageWithDelay(nanoseconds, message1.getBytes());
            }
            if (stop->getBuffer()[k]) {
                message1.setByte(0, TimedMessage::MessageType::MIDI_STOP);
                sendMessageWithDelay(nanoseconds, message1.getBytes());
            }
            if (clock->getBuffer()[k]) {
                message1.setByte(0, TimedMessage::MessageType::MIDI_TIME_CLOCK);
                sendMessageWithDelay(nanoseconds, message1.getBytes());
            }
        }
    }
    unlock();
}

void dsp::Midi::MidiOutput::sendMessageWithDelay(int64_t nanoseconds, std::vector<unsigned char> bytes) {
#if USE_RTMIDI
    std::thread([this, nanoseconds, bytes]() mutable {
        std::this_thread::sleep_for(std::chrono::nanoseconds(nanoseconds));
        midiOut.sendMessage(&bytes);
    }).detach();
#endif
}

#if USE_RTMIDI
RtMidiIn dsp::Midi::midiIn;
RtMidiOut dsp::Midi::midiOut;
#endif

unsigned int dsp::Midi::getNumMidiInputPorts() {
#if USE_RTMIDI
    return midiIn.getPortCount();
#else
    return 0;
#endif
}

unsigned int dsp::Midi::getNumMidiOutputPorts() {
#if USE_RTMIDI
    return midiOut.getPortCount();
#else
    return 0;
#endif
}

std::string dsp::Midi::getMidiInputName(unsigned int port) {
#if USE_RTMIDI
    return midiIn.getPortName(port);
#else
    return "None";
#endif
}

std::string dsp::Midi::getMidiOutputName(unsigned int port) {
#if USE_RTMIDI
    return midiOut.getPortName(port);
#else
    return "None";
#endif
}

dsp::Midi::Midi() {}

unsigned int dsp::Midi::getNumMidiInputs() {
    return static_cast<unsigned int>(midiInputs.size());
}

unsigned int dsp::Midi::getNumMidiOutputs() {
    return static_cast<unsigned int>(midiOutputs.size());
}

std::shared_ptr<dsp::Midi::MidiInput> dsp::Midi::getMidiInput(unsigned int index) {
    return midiInputs[index];
}

std::shared_ptr<dsp::Midi::MidiOutput> dsp::Midi::getMidiOutput(unsigned int index) {
    return midiOutputs[index];
}

void dsp::Midi::pushMidiInput(unsigned int port) {
    lock();
    midiInputs.push_back(std::make_shared<MidiInput>(port));
    unlock();
}

void dsp::Midi::pushMidiOutput(unsigned int port) {
    lock();
    midiOutputs.push_back(std::make_shared<MidiOutput>(port));
    unlock();
}

void dsp::Midi::removeMidiInput(unsigned int index) {
    lock();
    midiInputs.erase(midiInputs.begin() + index);
    unlock();
}

void dsp::Midi::removeMidiOutput(unsigned int index) {
    lock();
    midiOutputs.erase(midiOutputs.begin() + index);
    unlock();
}

void dsp::Midi::processInputs() {
    lock();
    for (const auto &input : midiInputs) {
        input->run();
    }
    unlock();
}

void dsp::Midi::processOutputs() {
    lock();
    for (const auto &output : midiOutputs) {
        output->run();
    }
    unlock();
}
