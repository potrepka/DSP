#include "Recorder.h"

dsp::Recorder::Recorder(Type type, Space space, Sample defaultValue)
        : Consumer(type, space)
        , mode(Mode::FIXED_SINGLE)
        , recordingTime(0.0)
        , recordingNumSamples(0.0)
        , primary(std::make_shared<Buffer>(type, space, 0.0, defaultValue, 0, 0))
        , secondary(std::make_shared<Buffer>(type, space, 0.0, defaultValue, 0, 0))
        , gate(std::make_shared<Input>(Type::BOOLEAN))
        , reset(std::make_shared<Input>(Type::BOOLEAN)) {
    getInputs().push_back(gate);
    getInputs().push_back(reset);
}

dsp::Recorder::Mode dsp::Recorder::getMode() const {
    return mode;
}

void dsp::Recorder::setMode(Mode mode) {
    lock();
    this->mode = mode;
    recordingTime = 0.0;
    recordingNumSamples = 0.0;
    std::fill(writeIndex.begin(), writeIndex.end(), 0.0);
    primary->setSize(0, 0);
    secondary->setSize(0, 0);
    unlock();
}

dsp::Sample dsp::Recorder::getRecordingTime() const {
    return recordingTime;
}

void dsp::Recorder::setRecordingTime(Sample seconds) {
    DSP_ASSERT(recordingTime >= 0.0);
    lock();
    setRecordingTimeNoLock(seconds);
    unlock();
}

std::shared_ptr<dsp::Buffer> dsp::Recorder::getRecordingBuffer() const {
    return primary;
}

std::shared_ptr<dsp::Input> dsp::Recorder::getGate() const {
    return gate;
}

std::shared_ptr<dsp::Input> dsp::Recorder::getReset() const {
    return reset;
}

void dsp::Recorder::setNumInputChannelsNoLock(size_t numChannels) {
    Node::setNumInputChannelsNoLock(numChannels);
    writeIndex.resize(numChannels, 0.0);
    primary->setNumChannels(numChannels);
    secondary->setNumChannels(numChannels);
}

void dsp::Recorder::setSampleRateNoLock(double sampleRate) {
    Node::setSampleRateNoLock(sampleRate);
    setRecordingTimeNoLock(recordingTime);
}

void dsp::Recorder::processNoLock() {
    primary->lock();
    secondary->lock();
    if (mode == Mode::VARIABLE) {
        recordingNumSamples += getNumSamples();
        recordingTime = recordingNumSamples * getOneOverSampleRate();
    }
    if (recordingNumSamples > 0.0) {
        for (size_t channel = 0; channel < getNumChannels(); ++channel) {
            Sample *inputChannel = getInput()->getWrapper().getChannelPointer(channel);
            Sample *gateChannel = getGate()->getWrapper().getChannelPointer(channel);
            Sample *resetChannel = getReset()->getWrapper().getChannelPointer(channel);
            for (size_t sample = 0; sample < getNumSamples(); ++sample) {
                if (resetChannel[sample]) {
                    writeIndex[channel] = 0.0;
                }
                if (gateChannel[sample]) {
                    switch (mode) {
                        case Mode::FIXED_SINGLE:
                            primary->getWrapper().getChannelPointer(channel)[static_cast<size_t>(writeIndex[channel])] =
                                    inputChannel[sample];
                            writeIndex[channel] += 1.0;
                            if (writeIndex[channel] >= recordingNumSamples) {
                                if (writeIndex[channel] < static_cast<int>(ceil(recordingNumSamples))) {
                                    primary->getWrapper().getChannelPointer(
                                            channel)[static_cast<int>(writeIndex[channel])] = inputChannel[sample];
                                }
                                writeIndex[channel] -= recordingNumSamples;
                            }
                            break;
                        case Mode::FIXED_DOUBLE:
                            secondary->getWrapper().getChannelPointer(
                                    channel)[static_cast<size_t>(writeIndex[channel])] = inputChannel[sample];
                            writeIndex[channel] += 1.0;
                            if (writeIndex[channel] >= recordingNumSamples) {
                                if (writeIndex[channel] < static_cast<int>(ceil(recordingNumSamples))) {
                                    primary->getWrapper().getChannelPointer(
                                            channel)[static_cast<int>(writeIndex[channel])] = inputChannel[sample];
                                }
                                writeIndex[channel] -= recordingNumSamples;
                                primary->getWrapper().copyFrom(secondary->getWrapper());
                            }
                            break;
                        case Mode::VARIABLE:
                            primary->getWrapper().getChannelPointer(channel)[static_cast<int>(writeIndex[channel])] =
                                    inputChannel[sample];
                            writeIndex[channel] += 1.0;
                            break;
                    }
                }
            }
        }
    }
    if (mode == Mode::VARIABLE) {
        recordingNumSamples = *std::max_element(writeIndex.begin(), writeIndex.end());
        recordingTime = recordingNumSamples * getOneOverSampleRate();
    }
    secondary->unlock();
    primary->unlock();
}

void dsp::Recorder::setRecordingTimeNoLock(Sample seconds) {
    recordingTime = seconds;
    recordingNumSamples = seconds * getSampleRate();
    std::fill(writeIndex.begin(), writeIndex.end(), 0.0);
    size_t numSamples = static_cast<int>(ceil(recordingNumSamples));
    primary->setNumSamples(numSamples);
    secondary->setNumSamples(numSamples);
}
