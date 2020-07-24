#include "Midi.h"

const unsigned char MIDI_CHANNELS = 16;
const unsigned char MIDI_NOTES = 128;

dsp::Midi::TimedMessage::TimedMessage(double time, std::vector<unsigned char> bytes) {
    this->time = time;
    this->bytes = bytes;
}

double dsp::Midi::TimedMessage::getTime() const {
    return time;
}

dsp::Midi::MessageType dsp::Midi::TimedMessage::getType() const {
    if (bytes[0] >= MIDI_SYSEX) {
        return (MessageType)bytes[0];
    } else {
        return (MessageType)(bytes[0] & 0xF0);
    }
}

std::string dsp::Midi::TimedMessage::getTypeName() const {
    return getMessageTypeName(getType());
}

unsigned char dsp::Midi::TimedMessage::getChannel() const {
    if (bytes[0] >= MIDI_SYSEX) {
        return 0;
    } else {
        return (unsigned int)(bytes[0] & 0x0F) + 1;
    }
}

std::vector<unsigned char> dsp::Midi::TimedMessage::getBytes() const {
    return bytes;
}

unsigned char dsp::Midi::TimedMessage::getByte(unsigned int index) const {
    assert(index < bytes.size());
    return bytes[index];
}

dsp::Sample dsp::Midi::TimedMessage::getByteAsUnipolar(unsigned int index) const {
    assert(index < bytes.size());
    return bytes[index] == 0 ? 0.0 : (bytes[index] + 1) * 0.0078125;
}

void dsp::Midi::TimedMessage::setByte(unsigned int index, unsigned char value) {
    assert(index < bytes.size());
    bytes[index] = value;
}

void dsp::Midi::TimedMessage::setByteUsingUnipolar(unsigned int index, Sample value) {
    assert(index < bytes.size());
    bytes[index] = value == 0.0 ? 0 : static_cast<unsigned char>(value * 128 - 1);
}

unsigned short dsp::Midi::TimedMessage::getShort(unsigned int lsb, unsigned int msb) const {
    assert(lsb < bytes.size() && msb < bytes.size());
    return bytes[lsb] + bytes[msb] * 128;
}

dsp::Sample dsp::Midi::TimedMessage::getShortAsBipolar(unsigned int lsb, unsigned int msb) const {
    assert(lsb < bytes.size() && msb < bytes.size());
    unsigned short x = getShort(lsb, msb) - 8192;
    return (x > 0 ? x + 1 : x) * 0.0001220703125;
}

void dsp::Midi::TimedMessage::setShort(unsigned int lsb, unsigned int msb, unsigned short value) {
    assert(lsb < bytes.size() && msb < bytes.size());
    unsigned char least = value % 128;
    unsigned char most = value / 128;
    bytes[lsb] = least;
    bytes[msb] = most;
}

void dsp::Midi::TimedMessage::setShortUsingBipolar(unsigned int lsb, unsigned int msb, Sample bipolar) {
    assert(lsb < bytes.size() && msb < bytes.size());
    setShort(lsb, msb, static_cast<unsigned short>(bipolar * 8192 + 8192));
}

void dsp::Midi::MidiInput::callback(double delta, std::vector<unsigned char> *bytesPointer, void *data) {
    MidiInput *input = (MidiInput *)data;
    input->pushQueue(delta, *bytesPointer);
}

dsp::Midi::MidiInput::MidiInput(unsigned int port)
        : messageTime(0.0)
        , bufferSamples(0) {
#if DSP_USE_RTMIDI
    try {
        midiIn.setCallback(MidiInput::callback, this);
    } catch (RtMidiError &error) {
#if DEBUG
        error.printMessage();
#endif
    }
#endif
    setPort(port);

    notePressureState.resize(MIDI_CHANNELS);
    controlChangeState.resize(MIDI_CHANNELS);
    channelPressureState.resize(MIDI_CHANNELS);
    pitchBendState.resize(MIDI_CHANNELS);

    noteOn.resize(MIDI_CHANNELS);
    noteOff.resize(MIDI_CHANNELS);
    notePressure.resize(MIDI_CHANNELS);
    controlChange.resize(MIDI_CHANNELS);
    channelPressure.resize(MIDI_CHANNELS);
    pitchBend.resize(MIDI_CHANNELS);

    play = std::make_shared<Input>(getBufferSize(), Type::BINARY);
    stop = std::make_shared<Input>(getBufferSize(), Type::BINARY);
    reset = std::make_shared<Input>(getBufferSize(), Type::BINARY);
    clock = std::make_shared<Input>(getBufferSize(), Type::BINARY);

    for (unsigned char i = 0; i < MIDI_CHANNELS; ++i) {
        notePressureState[i].resize(MIDI_NOTES);
        controlChangeState[i].resize(MIDI_NOTES);
        channelPressureState[i] = 0.0;
        pitchBendState[i] = 0.0;

        noteOn[i].resize(MIDI_NOTES);
        noteOff[i].resize(MIDI_NOTES);
        notePressure[i].resize(MIDI_NOTES);
        controlChange[i].resize(MIDI_NOTES);
        channelPressure[i] = std::make_shared<Input>(getBufferSize(), Type::UNIPOLAR);
        pitchBend[i] = std::make_shared<Input>(getBufferSize(), Type::BIPOLAR);

        for (unsigned char n = 0; n < MIDI_NOTES; n++) {
            notePressureState[i][n] = 0.0;
            controlChangeState[i][n] = 0.0;

            noteOn[i][n] = std::make_shared<Input>(getBufferSize(), Type::UNIPOLAR);
            noteOff[i][n] = std::make_shared<Input>(getBufferSize(), Type::UNIPOLAR);
            notePressure[i][n] = std::make_shared<Input>(getBufferSize(), Type::UNIPOLAR);
            controlChange[i][n] = std::make_shared<Input>(getBufferSize(), Type::UNIPOLAR);
        }
    }
}

std::string dsp::Midi::MidiInput::getDeviceName() const {
    return deviceName;
}

void dsp::Midi::MidiInput::setPort(unsigned int port) {
    lock();
#if DSP_USE_RTMIDI
    try {
        midiIn.openPort(port);
    } catch (RtMidiError &error) {
#if DEBUG
        error.printMessage();
#endif
    }
    deviceName = getMidiInputName(port);
#endif
    unlock();
}

std::shared_ptr<dsp::Input> dsp::Midi::MidiInput::getNoteOnTrigger(unsigned char channel, unsigned char note) const {
    assert(channel < noteOn.size() && note < noteOn[channel].size());
    return noteOn[channel][note];
}

std::shared_ptr<dsp::Input> dsp::Midi::MidiInput::getNoteOffTrigger(unsigned char channel, unsigned char note) const {
    assert(channel < noteOff.size() && note < noteOff[channel].size());
    return noteOff[channel][note];
}

std::shared_ptr<dsp::Input> dsp::Midi::MidiInput::getNotePressure(unsigned char channel, unsigned char note) const {
    assert(channel < notePressure.size() && note < notePressure[channel].size());
    return notePressure[channel][note];
}

std::shared_ptr<dsp::Input> dsp::Midi::MidiInput::getControlChange(unsigned char channel, unsigned char control) const {
    assert(channel < controlChange.size() && control < controlChange[channel].size());
    return controlChange[channel][control];
}

std::shared_ptr<dsp::Input> dsp::Midi::MidiInput::getChannelPressure(unsigned char channel) const {
    assert(channel < channelPressure.size());
    return channelPressure[channel];
}

std::shared_ptr<dsp::Input> dsp::Midi::MidiInput::getPitchBend(unsigned char channel) const {
    assert(channel < pitchBend.size());
    return pitchBend[channel];
}

std::shared_ptr<dsp::Input> dsp::Midi::MidiInput::getPlayTrigger() const {
    return play;
}

std::shared_ptr<dsp::Input> dsp::Midi::MidiInput::getStopTrigger() const {
    return stop;
}

std::shared_ptr<dsp::Input> dsp::Midi::MidiInput::getResetTrigger() const {
    return reset;
}

std::shared_ptr<dsp::Input> dsp::Midi::MidiInput::getClockTrigger() const {
    return clock;
}

unsigned int dsp::Midi::MidiInput::getNumCallbacks() const {
    return static_cast<unsigned int>(callbacks.size());
}

std::shared_ptr<std::function<void(dsp::Midi::TimedMessage)>>
dsp::Midi::MidiInput::getCallback(unsigned int index) const {
    assert(index < callbacks.size());
    return callbacks[index];
}

std::shared_ptr<std::function<void(dsp::Midi::TimedMessage)>>
dsp::Midi::MidiInput::pushCallback(std::function<void(TimedMessage)> callback) {
    std::shared_ptr<std::function<void(TimedMessage)>> pointer =
            std::make_shared<std::function<void(TimedMessage)>>(callback);
    lock();
    callbacks.push_back(pointer);
    unlock();
    return pointer;
}

void dsp::Midi::MidiInput::removeCallback(std::shared_ptr<std::function<void(Midi::TimedMessage)>> callback) {
    lock();
    callbacks.erase(std::remove(callbacks.begin(), callbacks.end(), callback), callbacks.end());
    unlock();
}

void dsp::Midi::MidiInput::run() {
    lock();
    for (unsigned int k = 0; k < getBufferSize(); ++k) {
        bool noteOnState = false;
        bool noteOffState = false;
        bool playState = false;
        bool stopState = false;
        bool resetState = false;
        bool clockState = false;
        if (queue.top().getTime() <= bufferSamples * getOneOverSampleRate()) {
            const TimedMessage &message = queue.top();
            const MessageType type = message.getType();
            const unsigned char channel = message.getChannel();
            switch (type) {
                case MIDI_NOTE_OFF: noteOffState = true; break;
                case MIDI_NOTE_ON:
                    noteOnState = true;
                    notePressureState[channel][message.getByte(1)] = message.getByteAsUnipolar(2);
                    break;
                case MIDI_POLY_AFTERTOUCH:
                    notePressureState[channel][message.getByte(1)] = message.getByteAsUnipolar(2);
                    break;
                case MIDI_CONTROL_CHANGE:
                    controlChangeState[channel][message.getByte(1)] = message.getByteAsUnipolar(2);
                    break;
                case MIDI_PROGRAM_CHANGE: break;
                case MIDI_AFTERTOUCH: channelPressureState[channel] = message.getByteAsUnipolar(1); break;
                case MIDI_PITCH_BEND: pitchBendState[channel] = message.getShortAsBipolar(1, 2); break;

                case MIDI_TIME_CODE:
                case MIDI_SONG_POS_POINTER:
                case MIDI_SONG_SELECT:
                case MIDI_TUNE_REQUEST: break;

                case MIDI_TIME_CLOCK: clockState = true; break;
                case MIDI_START:
                    playState = true;
                    resetState = true;
                    break;
                case MIDI_CONTINUE: playState = true; break;
                case MIDI_STOP: stopState = true; break;

                case MIDI_UNKNOWN:
                case MIDI_SYSEX:
                case MIDI_SYSEX_END:
                case MIDI_ACTIVE_SENSING:
                case MIDI_SYSTEM_RESET: break;
            }
        }
        for (unsigned char i = 0; i < MIDI_CHANNELS; ++i) {
            for (unsigned char n = 0; n < MIDI_NOTES; n++) {
                noteOn[i][n]->getBuffer()[k] = noteOnState ? 1.0 : 0.0;
                noteOff[i][n]->getBuffer()[k] = noteOffState ? 1.0 : 0.0;
                notePressure[i][n]->getBuffer()[k] = notePressureState[i][n];
                controlChange[i][n]->getBuffer()[k] = controlChangeState[i][n];
            }
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
    TimedMessage message(messageTime += delta, bytes);
    lock();
    runCallbacks(message);
    queue.push(message);
    unlock();
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
    channelPressureState.resize(MIDI_CHANNELS);
    pitchBendState.resize(MIDI_CHANNELS);

    noteOn.resize(MIDI_CHANNELS);
    noteOff.resize(MIDI_CHANNELS);
    notePressure.resize(MIDI_CHANNELS);
    controlChange.resize(MIDI_CHANNELS);
    channelPressure.resize(MIDI_CHANNELS);
    pitchBend.resize(MIDI_CHANNELS);

    play = std::make_shared<Output>(getBufferSize(), Type::BINARY);
    stop = std::make_shared<Output>(getBufferSize(), Type::BINARY);
    reset = std::make_shared<Output>(getBufferSize(), Type::BINARY);
    clock = std::make_shared<Output>(getBufferSize(), Type::BINARY);

    for (unsigned char i = 0; i < MIDI_CHANNELS; ++i) {
        notePressureState[i].resize(MIDI_NOTES);
        controlChangeState[i].resize(MIDI_NOTES);
        channelPressureState[i] = 0;
        pitchBendState[i] = 0;

        noteOn[i].resize(MIDI_NOTES);
        noteOff[i].resize(MIDI_NOTES);
        notePressure[i].resize(MIDI_NOTES);
        controlChange[i].resize(MIDI_NOTES);
        channelPressure[i] = std::make_shared<Output>(getBufferSize(), Type::UNIPOLAR);
        pitchBend[i] = std::make_shared<Output>(getBufferSize(), Type::BIPOLAR);

        for (unsigned char n = 0; n < MIDI_NOTES; n++) {
            notePressureState[i][n] = 0;
            controlChangeState[i][n] = 0;

            noteOn[i][n] = std::make_shared<Output>(getBufferSize(), Type::UNIPOLAR);
            noteOff[i][n] = std::make_shared<Output>(getBufferSize(), Type::UNIPOLAR);
            notePressure[i][n] = std::make_shared<Output>(getBufferSize(), Type::UNIPOLAR);
            controlChange[i][n] = std::make_shared<Output>(getBufferSize(), Type::UNIPOLAR);
        }
    }
}

std::string dsp::Midi::MidiOutput::getDeviceName() const {
    return deviceName;
}

void dsp::Midi::MidiOutput::setPort(unsigned int port) {
    lock();
#if DSP_USE_RTMIDI
    try {
        midiOut.openPort(port);
    } catch (RtMidiError &error) {
#if DEBUG
        error.printMessage();
#endif
    }
    deviceName = getMidiOutputName(port);
#endif
    unlock();
}

std::shared_ptr<dsp::Output> dsp::Midi::MidiOutput::getNoteOnTrigger(unsigned char channel, unsigned char note) const {
    assert(channel < noteOn.size() && note < noteOn[channel].size());
    return noteOn[channel][note];
}

std::shared_ptr<dsp::Output> dsp::Midi::MidiOutput::getNoteOffTrigger(unsigned char channel, unsigned char note) const {
    assert(channel < noteOff.size() && note < noteOff[channel].size());
    return noteOff[channel][note];
}

std::shared_ptr<dsp::Output> dsp::Midi::MidiOutput::getNotePressure(unsigned char channel, unsigned char note) const {
    assert(channel < notePressure.size() && note < notePressure[channel].size());
    return notePressure[channel][note];
}

std::shared_ptr<dsp::Output> dsp::Midi::MidiOutput::getControlChange(unsigned char channel,
                                                                     unsigned char control) const {
    assert(channel < controlChange.size() && control < controlChange[channel].size());
    return controlChange[channel][control];
}

std::shared_ptr<dsp::Output> dsp::Midi::MidiOutput::getChannelPressure(unsigned char channel) const {
    assert(channel < channelPressure.size());
    return channelPressure[channel];
}

std::shared_ptr<dsp::Output> dsp::Midi::MidiOutput::getPitchBend(unsigned char channel) const {
    assert(channel < pitchBend.size());
    return pitchBend[channel];
}

std::shared_ptr<dsp::Output> dsp::Midi::MidiOutput::getPlayTrigger() const {
    return play;
}

std::shared_ptr<dsp::Output> dsp::Midi::MidiOutput::getStopTrigger() const {
    return stop;
}

std::shared_ptr<dsp::Output> dsp::Midi::MidiOutput::getResetTrigger() const {
    return reset;
}

std::shared_ptr<dsp::Output> dsp::Midi::MidiOutput::getClockTrigger() const {
    return clock;
}

void dsp::Midi::MidiOutput::run() {
    lock();
    TimedMessage message1(0.0, std::vector<unsigned char>(1));
    TimedMessage message2(0.0, std::vector<unsigned char>(2));
    TimedMessage message3(0.0, std::vector<unsigned char>(3));
    for (unsigned int k = 0; k < getBufferSize(); ++k) {
        int64_t nanoseconds = static_cast<int64_t>(1000000000 * getOneOverSampleRate() * k);
        for (unsigned char i = 0; i < MIDI_CHANNELS; ++i) {
            for (unsigned char n = 0; n < MIDI_NOTES; n++) {
                if (noteOff[i][n]->getBuffer()[k]) {
                    message3.setByte(0, MIDI_NOTE_OFF);
                    message3.setByte(1, n);
                    message3.setByte(2, 0);
                    sendMessageWithDelay(nanoseconds, message3.getBytes());
                }
                if (noteOn[i][n]->getBuffer()[k]) {
                    message3.setByte(0, MIDI_NOTE_ON);
                    message3.setByte(1, n);
                    message3.setByteUsingUnipolar(2, notePressure[i][n]->getBuffer()[k]);
                    sendMessageWithDelay(nanoseconds, message3.getBytes());
                } else {
                    message3.setByteUsingUnipolar(2, notePressure[i][n]->getBuffer()[k]);
                    if (notePressureState[i][n] != message3.getByte(2)) {
                        notePressureState[i][n] = message3.getByte(2);
                        message3.setByte(0, MIDI_POLY_AFTERTOUCH + i);
                        message3.setByte(1, n);
                        sendMessageWithDelay(nanoseconds, message3.getBytes());
                    }
                }
                message3.setByteUsingUnipolar(2, controlChange[i][n]->getBuffer()[k]);
                if (controlChangeState[i][n] != message3.getByte(2)) {
                    controlChangeState[i][n] = message3.getByte(2);
                    message3.setByte(0, MIDI_CONTROL_CHANGE + i);
                    message3.setByte(1, n);
                    sendMessageWithDelay(nanoseconds, message3.getBytes());
                }
            }
            message2.setByteUsingUnipolar(1, channelPressure[i]->getBuffer()[k]);
            if (channelPressureState[i] != message2.getByte(1)) {
                channelPressureState[i] = message2.getByte(1);
                message2.setByte(0, MIDI_AFTERTOUCH + i);
                sendMessageWithDelay(nanoseconds, message2.getBytes());
            }
            message3.setShortUsingBipolar(1, 2, pitchBend[i]->getBuffer()[k]);
            if (pitchBendState[i] != message3.getShort(1, 2)) {
                pitchBendState[i] = message3.getShort(1, 2);
                message3.setByte(0, MIDI_PITCH_BEND + i);
                sendMessageWithDelay(nanoseconds, message3.getBytes());
            }
            if (play->getBuffer()[k]) {
                message1.setByte(0, reset->getBuffer()[k] ? MIDI_START : MIDI_CONTINUE);
                sendMessageWithDelay(nanoseconds, message1.getBytes());
            }
            if (stop->getBuffer()[k]) {
                message1.setByte(0, MIDI_STOP);
                sendMessageWithDelay(nanoseconds, message1.getBytes());
            }
            if (clock->getBuffer()[k]) {
                message1.setByte(0, MIDI_TIME_CLOCK);
                sendMessageWithDelay(nanoseconds, message1.getBytes());
            }
        }
    }
    unlock();
}

void dsp::Midi::MidiOutput::sendMessageWithDelay(int64_t nanoseconds, std::vector<unsigned char> bytes) {
#if DSP_USE_RTMIDI
    std::thread([this, nanoseconds, bytes]() mutable {
        std::this_thread::sleep_for(std::chrono::nanoseconds(nanoseconds));
        try {
            midiOut.sendMessage(&bytes);
        } catch (RtMidiError &error) {
#if DEBUG
            error.printMessage();
#endif
        }
    }).detach();
#endif
}

#if DSP_USE_RTMIDI
RtMidiIn dsp::Midi::midiIn;
RtMidiOut dsp::Midi::midiOut;
#endif

std::string dsp::Midi::getMessageTypeName(MessageType type) {
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

unsigned int dsp::Midi::getNumMidiInputPorts() {
#if DSP_USE_RTMIDI
    unsigned int portCount;
    try {
        portCount = midiIn.getPortCount();
    } catch (RtMidiError &error) {
        portCount = 0;
#if DEBUG
        error.printMessage();
#endif
    }
    return portCount;
#else
    return 0;
#endif
}

unsigned int dsp::Midi::getNumMidiOutputPorts() {
#if DSP_USE_RTMIDI
    unsigned int portCount;
    try {
        portCount = midiOut.getPortCount();
    } catch (RtMidiError &error) {
        portCount = 0;
#if DEBUG
        error.printMessage();
#endif
    }
    return portCount;
#else
    return 0;
#endif
}

std::string dsp::Midi::getMidiInputName(unsigned int port) {
#if DSP_USE_RTMIDI
    std::string name;
    try {
        name = midiIn.getPortName(port);
    } catch (RtMidiError &error) {
        name = "None";
#if DEBUG
        error.printMessage();
#endif
    }
    return name;
#else
    return "None";
#endif
}

std::string dsp::Midi::getMidiOutputName(unsigned int port) {
#if DSP_USE_RTMIDI
    std::string name;
    try {
        name = midiOut.getPortName(port);
    } catch (RtMidiError &error) {
        name = "None";
#if DEBUG
        error.printMessage();
#endif
    }
    return name;
#else
    return "None";
#endif
}

unsigned int dsp::Midi::getNumMidiInputs() const {
    return static_cast<unsigned int>(midiInputs.size());
}

unsigned int dsp::Midi::getNumMidiOutputs() const {
    return static_cast<unsigned int>(midiOutputs.size());
}

std::shared_ptr<dsp::Midi::MidiInput> dsp::Midi::getMidiInput(unsigned int index) const {
    assert(index < midiInputs.size());
    return midiInputs[index];
}

std::shared_ptr<dsp::Midi::MidiOutput> dsp::Midi::getMidiOutput(unsigned int index) const {
    assert(index < midiOutputs.size());
    return midiOutputs[index];
}

std::shared_ptr<dsp::Midi::MidiInput> dsp::Midi::pushMidiInput(unsigned int port) {
    auto input = std::make_shared<MidiInput>(port);
    lock();
    midiInputs.push_back(input);
    unlock();
    return input;
}

std::shared_ptr<dsp::Midi::MidiOutput> dsp::Midi::pushMidiOutput(unsigned int port) {
    auto output = std::make_shared<MidiOutput>(port);
    lock();
    midiOutputs.push_back(output);
    unlock();
    return output;
}

void dsp::Midi::removeMidiInput(std::shared_ptr<MidiInput> input) {
    lock();
    midiInputs.erase(std::remove(midiInputs.begin(), midiInputs.end(), input), midiInputs.end());
    unlock();
}

void dsp::Midi::removeMidiOutput(std::shared_ptr<MidiOutput> output) {
    lock();
    midiOutputs.erase(std::remove(midiOutputs.begin(), midiOutputs.end(), output), midiOutputs.end());
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
