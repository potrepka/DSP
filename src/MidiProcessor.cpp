#include "MidiProcessor.h"

void dsp::MidiProcessor::Input::callback(double delta, std::vector<unsigned char> *bytesPointer, void *data) {
    Input *input = reinterpret_cast<Input *>(data);
    input->lock();
    input->messages.insert({input->messageTime += delta, MidiMessage(*bytesPointer)});
    input->unlock();
}

dsp::MidiProcessor::Input::Input(unsigned int port)
        : messageTime(0.0) {
#if DSP_USE_RTMIDI
    try {
        midiIn.setCallback(callback, this);
    } catch (RtMidiError &error) {
#if DEBUG
        error.printMessage();
#endif
    }
#endif
    setPort(port);
}

std::string dsp::MidiProcessor::Input::getDeviceName() const {
    return deviceName;
}

void dsp::MidiProcessor::Input::setPort(unsigned int port) {
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

std::multimap<double, dsp::MidiMessage> &dsp::MidiProcessor::Input::getMessages() {
    return messages;
}

dsp::MidiProcessor::Output::Output(unsigned int port) {
    setPort(port);
}

std::string dsp::MidiProcessor::Output::getDeviceName() const {
    return deviceName;
}

void dsp::MidiProcessor::Output::setPort(unsigned int port) {
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

void dsp::MidiProcessor::Output::sendMessageWithDelay(std::vector<unsigned char> bytes, int64_t nanoseconds) {
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
RtMidiIn dsp::MidiProcessor::midiIn;
RtMidiOut dsp::MidiProcessor::midiOut;
#endif

unsigned int dsp::MidiProcessor::getNumMidiInputPorts() {
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

unsigned int dsp::MidiProcessor::getNumMidiOutputPorts() {
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

std::string dsp::MidiProcessor::getMidiInputName(unsigned int port) {
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

std::string dsp::MidiProcessor::getMidiOutputName(unsigned int port) {
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

dsp::MidiProcessor::MidiProcessor()
        : numSamples(0)
        , sampleRate(0.0)
        , oneOverSampleRate(0.0)
        , messageTime(0.0) {}

int dsp::MidiProcessor::getNumSamples() const {
    return numSamples;
}

void dsp::MidiProcessor::setNumSamples(int numSamples) {
    assert(numSamples >= 0);
    lock();
    this->numSamples = numSamples;
    unlock();
}

double dsp::MidiProcessor::getSampleRate() const {
    return sampleRate;
}

void dsp::MidiProcessor::setSampleRate(double sampleRate) {
    assert(sampleRate >= 0);
    lock();
    this->sampleRate = sampleRate;
    oneOverSampleRate = 1.0 / sampleRate;
    unlock();
}

std::vector<std::shared_ptr<dsp::MidiProcessor::Input>> &dsp::MidiProcessor::getInputs() {
    return inputs;
}

std::vector<std::shared_ptr<dsp::MidiProcessor::Output>> &dsp::MidiProcessor::getOutputs() {
    return outputs;
}

dsp::MidiData &dsp::MidiProcessor::getMidiData() {
    return midiData;
}

void dsp::MidiProcessor::processInputs() {
    lock();
    midiData.clear();
    for (const auto &input : inputs) {
        input->lock();
        for (const auto &pair : input->getMessages()) {
            if (pair.first == 0.0) {
                messageTime = 0.0;
            }
            int timeInBuffer = static_cast<int>((pair.first - messageTime) * sampleRate);
            if (timeInBuffer < 0) {
                timeInBuffer = 0;
            }
            if (timeInBuffer >= numSamples) {
                timeInBuffer = numSamples - 1;
            }
            MidiMessage message = pair.second;
            midiData.addEvent(message, timeInBuffer);
        }
        input->getMessages().clear();
        input->unlock();
    }
    unlock();
}

void dsp::MidiProcessor::processOutputs() {
    lock();
    for (const auto &output : outputs) {
        output->lock();
        for (const auto &meta : midiData) {
            MidiMessage message = meta.getMessage();
            output->sendMessageWithDelay(message.getBytes(), meta.samplePosition * oneOverSampleRate);
        }
        output->unlock();
    }
    messageTime += numSamples * oneOverSampleRate;
    unlock();
}
