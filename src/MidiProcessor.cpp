#include "MidiProcessor.h"

void dsp::MidiProcessor::Input::callback(double delta, std::vector<unsigned char> *bytesPointer, void *data) {
    Input *input = reinterpret_cast<Input *>(data);
    input->lock();
    input->messages.insert({input->messageTime += delta, MidiMessage(bytesPointer->data(), bytesPointer->size())});
    input->unlock();
}

dsp::MidiProcessor::Input::Input(unsigned int port)
        : messageTime(0.0) {
#ifdef DSP_USE_RTMIDI
    try {
        midiIn.setCallback(callback, this);
    } catch (RtMidiError &error) {
#if defined(DEBUG) || defined(_DEBUG)
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
#ifdef DSP_USE_RTMIDI
    try {
        midiIn.openPort(port);
    } catch (RtMidiError &error) {
#if defined(DEBUG) || defined(_DEBUG)
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
#ifdef DSP_USE_RTMIDI
    try {
        midiOut.openPort(port);
    } catch (RtMidiError &error) {
#if defined(DEBUG) || defined(_DEBUG)
        error.printMessage();
#endif
    }
    deviceName = getMidiOutputName(port);
#endif
    unlock();
}

void dsp::MidiProcessor::Output::sendMessageWithDelay(std::vector<unsigned char> bytes, int64_t nanoseconds) {
#ifdef DSP_USE_RTMIDI
    std::thread([this, nanoseconds, bytes]() mutable {
        std::this_thread::sleep_for(std::chrono::nanoseconds(nanoseconds));
        try {
            midiOut.sendMessage(&bytes);
        } catch (RtMidiError &error) {
#if defined(DEBUG) || defined(_DEBUG)
            error.printMessage();
#endif
        }
    }).detach();
#endif
}

#ifdef DSP_USE_RTMIDI
RtMidiIn dsp::MidiProcessor::midiIn;
RtMidiOut dsp::MidiProcessor::midiOut;
#endif

unsigned int dsp::MidiProcessor::getNumMidiInputPorts() {
#ifdef DSP_USE_RTMIDI
    unsigned int portCount;
    try {
        portCount = midiIn.getPortCount();
    } catch (RtMidiError &error) {
        portCount = 0;
#if defined(DEBUG) || defined(_DEBUG)
        error.printMessage();
#endif
    }
    return portCount;
#else
    return 0;
#endif
}

unsigned int dsp::MidiProcessor::getNumMidiOutputPorts() {
#ifdef DSP_USE_RTMIDI
    unsigned int portCount;
    try {
        portCount = midiOut.getPortCount();
    } catch (RtMidiError &error) {
        portCount = 0;
#if defined(DEBUG) || defined(_DEBUG)
        error.printMessage();
#endif
    }
    return portCount;
#else
    return 0;
#endif
}

std::string dsp::MidiProcessor::getMidiInputName(unsigned int port) {
#ifdef DSP_USE_RTMIDI
    std::string name;
    try {
        name = midiIn.getPortName(port);
    } catch (RtMidiError &error) {
        name = "None";
#if defined(DEBUG) || defined(_DEBUG)
        error.printMessage();
#endif
    }
    return name;
#else
    return "None";
#endif
}

std::string dsp::MidiProcessor::getMidiOutputName(unsigned int port) {
#ifdef DSP_USE_RTMIDI
    std::string name;
    try {
        name = midiOut.getPortName(port);
    } catch (RtMidiError &error) {
        name = "None";
#if defined(DEBUG) || defined(_DEBUG)
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

size_t dsp::MidiProcessor::getNumSamples() const {
    return numSamples;
}

void dsp::MidiProcessor::setNumSamples(size_t numSamples) {
    lock();
    this->numSamples = numSamples;
    unlock();
}

double dsp::MidiProcessor::getSampleRate() const {
    return sampleRate;
}

void dsp::MidiProcessor::setSampleRate(double sampleRate) {
    DSP_ASSERT(sampleRate >= 0.0);
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

dsp::MidiBuffer &dsp::MidiProcessor::getMidiBuffer() {
    return midiBuffer;
}

void dsp::MidiProcessor::processInputs() {
    lock();
    midiBuffer.clear();
    for (const auto &input : inputs) {
        input->lock();
        for (const auto &pair : input->getMessages()) {
            if (pair.first == 0.0) {
                messageTime = 0.0;
            }
            size_t timeInBuffer = static_cast<size_t>((pair.first - messageTime) * sampleRate);
            if (timeInBuffer < 0) {
                timeInBuffer = 0;
            }
            if (timeInBuffer >= numSamples) {
                timeInBuffer = numSamples - 1;
            }
            MidiMessage message = pair.second;
            midiBuffer.addEvent(message, timeInBuffer);
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
        for (const auto meta : midiBuffer) {
            MidiMessage message = meta.getMessage();
            auto data = message.getRawData();
            auto size = message.getRawDataSize();
            std::vector<uint8_t> bytes(size);
            for (size_t i = 0; i < size; ++i) {
                bytes.push_back(data[i]);
            }
            output->sendMessageWithDelay(bytes, meta.samplePosition * oneOverSampleRate);
        }
        output->unlock();
    }
    messageTime += numSamples * oneOverSampleRate;
    unlock();
}
