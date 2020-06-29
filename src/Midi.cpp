#include "Midi.h"

const unsigned char MIDI_CHANNELS = 16;
const unsigned char MIDI_NOTES = 128;

dsp::Midi::TimedMessage::TimedMessage(double time, std::vector<unsigned char> message) {
    this->time = time;
    this->message = message;
}

double dsp::Midi::TimedMessage::getTime() const {
    return time;
}

std::vector<unsigned char> dsp::Midi::TimedMessage::getMessage() const {
    return message;
}

dsp::Midi::TimedMessage::Type dsp::Midi::TimedMessage::getType() const {
    if (message[0] >= MIDI_SYSEX) {
        return (Type)message[0];
    } else {
        return (Type)(message[0] & 0xF0);
    }
}

unsigned char dsp::Midi::TimedMessage::getChannel() const {
    if (message[0] >= MIDI_SYSEX) {
        return 0;
    } else {
        return (unsigned int)(message[0] & 0x0F) + 1;
    }
}

unsigned char dsp::Midi::TimedMessage::getByte(std::size_t index) const {
    return message[index];
}

DSP_FLOAT dsp::Midi::TimedMessage::getByteAsUnipolar(std::size_t index) const {
    return message[index] == 0 ? 0.0 : (message[index] + 1) * 0.0078125;
}

void dsp::Midi::TimedMessage::setByte(std::size_t index, unsigned char value) {
    message[index] = value;
}

void dsp::Midi::TimedMessage::setByteUsingUnipolar(std::size_t index, DSP_FLOAT value) {
    message[index] = value == 0.0 ? 0 : static_cast<unsigned char>(value * 128 - 1);
}

unsigned short dsp::Midi::TimedMessage::getShort(std::size_t lsb, std::size_t msb) const {
    return message[lsb] + message[msb] * 128;
}

DSP_FLOAT dsp::Midi::TimedMessage::getShortAsBipolar(std::size_t lsb, std::size_t msb) const {
    unsigned short x = getShort(lsb, msb) - 8192;
    return (x > 0 ? x + 1 : x) * 0.0001220703125;
}

void dsp::Midi::TimedMessage::setShort(std::size_t lsb, std::size_t msb, unsigned short value) {
    unsigned char least = value % 128;
    unsigned char most = value / 128;
    message[lsb] = least;
    message[msb] = most;
}

void dsp::Midi::TimedMessage::setShortUsingBipolar(std::size_t lsb, std::size_t msb, DSP_FLOAT bipolar) {
    setShort(lsb, msb, static_cast<unsigned short>(bipolar * 8192 + 8192));
}

void dsp::Midi::MidiInput::callback(double delta, std::vector<unsigned char> *message, void *data) {
    MidiInput *input = (MidiInput *)data;
    input->pushQueue(delta, *message);
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

    play = std::make_shared<Input>(getBufferSize(), Connection::Type::BINARY, Connection::Space::TIME, 0.0);
    stop = std::make_shared<Input>(getBufferSize(), Connection::Type::BINARY, Connection::Space::TIME, 0.0);
    reset = std::make_shared<Input>(getBufferSize(), Connection::Type::BINARY, Connection::Space::TIME, 0.0);
    clock = std::make_shared<Input>(getBufferSize(), Connection::Type::BINARY, Connection::Space::TIME, 0.0);

    for (unsigned char i = 0; i < MIDI_CHANNELS; i++) {
        notePressureState[i].reserve(MIDI_NOTES);
        controlChangeState[i].reserve(MIDI_NOTES);
        programChangeState[i] = 0.0;
        channelPressureState[i] = 0.0;
        pitchBendState[i] = 0.0;

        noteOn[i].reserve(MIDI_NOTES);
        noteOff[i].reserve(MIDI_NOTES);
        notePressure[i].reserve(MIDI_NOTES);
        controlChange[i].reserve(MIDI_NOTES);
        programChange[i] =
                std::make_shared<Input>(getBufferSize(), Connection::Type::INTEGER, Connection::Space::TIME, 0.0);
        channelPressure[i] =
                std::make_shared<Input>(getBufferSize(), Connection::Type::UNIPOLAR, Connection::Space::TIME, 0.0);
        pitchBend[i] =
                std::make_shared<Input>(getBufferSize(), Connection::Type::BIPOLAR, Connection::Space::TIME, 0.0);

        for (unsigned char n = 0; n < MIDI_NOTES; n++) {
            notePressureState[i][n] = 0.0;
            controlChangeState[i][n] = 0.0;

            noteOn[i][n] =
                    std::make_shared<Input>(getBufferSize(), Connection::Type::UNIPOLAR, Connection::Space::TIME, 0.0);
            noteOff[i][n] =
                    std::make_shared<Input>(getBufferSize(), Connection::Type::UNIPOLAR, Connection::Space::TIME, 0.0);
            notePressure[i][n] =
                    std::make_shared<Input>(getBufferSize(), Connection::Type::UNIPOLAR, Connection::Space::TIME, 0.0);
            controlChange[i][n] =
                    std::make_shared<Input>(getBufferSize(), Connection::Type::UNIPOLAR, Connection::Space::TIME, 0.0);
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

void dsp::Midi::MidiInput::pushQueue(double delta, std::vector<unsigned char> &message) {
    messageTime += delta;
    queue.push(TimedMessage(messageTime, message));
}

void dsp::Midi::MidiInput::runCallbacks(double delta, std::vector<unsigned char> &message) {
    for (const auto &callback : callbacks) {
        callback(delta, message);
    }
}

void dsp::Midi::MidiInput::addCallback(void (*callback)(double, std::vector<unsigned char>)) {
    callbacks.push_back(callback);
}

void dsp::Midi::MidiInput::removeCallback(void (*callback)(double, std::vector<unsigned char>)) {
    callbacks.erase(std::remove(callbacks.begin(), callbacks.end(), callback), callbacks.end());
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
            const TimedMessage::Type type = message.getType();
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

    play = std::make_shared<Output>(getBufferSize(), Connection::Type::BINARY, Connection::Space::TIME, 0.0);
    stop = std::make_shared<Output>(getBufferSize(), Connection::Type::BINARY, Connection::Space::TIME, 0.0);
    reset = std::make_shared<Output>(getBufferSize(), Connection::Type::BINARY, Connection::Space::TIME, 0.0);
    clock = std::make_shared<Output>(getBufferSize(), Connection::Type::BINARY, Connection::Space::TIME, 0.0);

    for (unsigned char i = 0; i < MIDI_CHANNELS; i++) {
        notePressureState[i].reserve(MIDI_NOTES);
        controlChangeState[i].reserve(MIDI_NOTES);
        programChangeState[i] = 0;
        channelPressureState[i] = 0;
        pitchBendState[i] = 0;

        noteOn[i].reserve(MIDI_NOTES);
        noteOff[i].reserve(MIDI_NOTES);
        notePressure[i].reserve(MIDI_NOTES);
        controlChange[i].reserve(MIDI_NOTES);
        programChange[i] =
                std::make_shared<Output>(getBufferSize(), Connection::Type::INTEGER, Connection::Space::TIME, 0.0);
        channelPressure[i] =
                std::make_shared<Output>(getBufferSize(), Connection::Type::UNIPOLAR, Connection::Space::TIME, 0.0);
        pitchBend[i] =
                std::make_shared<Output>(getBufferSize(), Connection::Type::BIPOLAR, Connection::Space::TIME, 0.0);

        for (unsigned char n = 0; n < MIDI_NOTES; n++) {
            notePressureState[i][n] = 0;
            controlChangeState[i][n] = 0;

            noteOn[i][n] =
                    std::make_shared<Output>(getBufferSize(), Connection::Type::UNIPOLAR, Connection::Space::TIME, 0.0);
            noteOff[i][n] =
                    std::make_shared<Output>(getBufferSize(), Connection::Type::UNIPOLAR, Connection::Space::TIME, 0.0);
            notePressure[i][n] =
                    std::make_shared<Output>(getBufferSize(), Connection::Type::UNIPOLAR, Connection::Space::TIME, 0.0);
            controlChange[i][n] =
                    std::make_shared<Output>(getBufferSize(), Connection::Type::UNIPOLAR, Connection::Space::TIME, 0.0);
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

void dsp::Midi::MidiOutput::sendMessageWithDelay(int64_t nanoseconds, std::vector<unsigned char> message) {
#if USE_RTMIDI
    std::thread([this, nanoseconds, message]() mutable {
        std::this_thread::sleep_for(std::chrono::nanoseconds(nanoseconds));
        midiOut.sendMessage(&message);
    }).detach();
#endif
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
                    message3.setByte(0, TimedMessage::Type::MIDI_NOTE_OFF);
                    message3.setByte(1, n);
                    message3.setByte(2, 0);
                    sendMessageWithDelay(nanoseconds, message3.getMessage());
                }
                if (noteOn[i][n]->getBuffer()[k]) {
                    message3.setByte(0, TimedMessage::Type::MIDI_NOTE_ON);
                    message3.setByte(1, n);
                    message3.setByteUsingUnipolar(2, notePressure[i][n]->getBuffer()[k]);
                    sendMessageWithDelay(nanoseconds, message3.getMessage());
                } else {
                    message3.setByteUsingUnipolar(2, notePressure[i][n]->getBuffer()[k]);
                    if (notePressureState[i][n] != message3.getByte(2)) {
                        notePressureState[i][n] = message3.getByte(2);
                        message3.setByte(0, TimedMessage::Type::MIDI_POLY_AFTERTOUCH + i);
                        message3.setByte(1, n);
                        sendMessageWithDelay(nanoseconds, message3.getMessage());
                    }
                }
                message3.setByteUsingUnipolar(2, controlChange[i][n]->getBuffer()[k]);
                if (controlChangeState[i][n] != message3.getByte(2)) {
                    controlChangeState[i][n] = message3.getByte(2);
                    message3.setByte(0, TimedMessage::Type::MIDI_CONTROL_CHANGE + i);
                    message3.setByte(1, n);
                    sendMessageWithDelay(nanoseconds, message3.getMessage());
                }
            }
            message2.setByte(1, programChange[i]->getBuffer()[k]);
            if (programChangeState[i] != message2.getByte(1)) {
                programChangeState[i] = message2.getByte(1);
                message2.setByte(0, TimedMessage::Type::MIDI_PROGRAM_CHANGE + i);
                sendMessageWithDelay(nanoseconds, message2.getMessage());
            }
            message2.setByteUsingUnipolar(1, channelPressure[i]->getBuffer()[k]);
            if (channelPressureState[i] != message2.getByte(1)) {
                channelPressureState[i] = message2.getByte(1);
                message2.setByte(0, TimedMessage::Type::MIDI_AFTERTOUCH + i);
                sendMessageWithDelay(nanoseconds, message2.getMessage());
            }
            message3.setShortUsingBipolar(1, 2, pitchBend[i]->getBuffer()[k]);
            if (pitchBendState[i] != message3.getShort(1, 2)) {
                pitchBendState[i] = message3.getShort(1, 2);
                message3.setByte(0, TimedMessage::Type::MIDI_PITCH_BEND + i);
                sendMessageWithDelay(nanoseconds, message3.getMessage());
            }
            if (play->getBuffer()[k]) {
                message1.setByte(0, reset->getBuffer()[k] ? TimedMessage::Type::MIDI_START
                                                          : TimedMessage::Type::MIDI_CONTINUE);
                sendMessageWithDelay(nanoseconds, message1.getMessage());
            }
            if (stop->getBuffer()[k]) {
                message1.setByte(0, TimedMessage::Type::MIDI_STOP);
                sendMessageWithDelay(nanoseconds, message1.getMessage());
            }
            if (clock->getBuffer()[k]) {
                message1.setByte(0, TimedMessage::Type::MIDI_TIME_CLOCK);
                sendMessageWithDelay(nanoseconds, message1.getMessage());
            }
        }
    }
    unlock();
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

std::size_t dsp::Midi::getNumMidiInputs() {
    return midiInputs.size();
}

std::size_t dsp::Midi::getNumMidiOutputs() {
    return midiOutputs.size();
}

std::shared_ptr<dsp::Midi::MidiInput> dsp::Midi::getMidiInput(std::size_t index) {
    return midiInputs[index];
}

std::shared_ptr<dsp::Midi::MidiOutput> dsp::Midi::getMidiOutput(std::size_t index) {
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

void dsp::Midi::removeMidiInput(std::size_t index) {
    lock();
    midiInputs.erase(midiInputs.begin() + index);
    unlock();
}

void dsp::Midi::removeMidiOutput(std::size_t index) {
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
