#include "BufferRecorder.h"

dsp::BufferRecorder::BufferRecorder(Type type, Space space, DSP_FLOAT defaultValue)
        : Consumer(type, space)
        , mode(Mode::SINGLE_BUFFER)
        , externalBufferSize(0)
        , externalBufferSizeSynced(true)
        , writeIndex(0) {
    buffer = std::make_shared<Buffer>(0, 0, type, space, defaultValue);
    second = std::make_shared<Buffer>(0, 0, type, space, defaultValue);
}

dsp::BufferRecorder::Mode dsp::BufferRecorder::getMode() const {
    return mode;
}

void dsp::BufferRecorder::setMode(Mode mode) {
    lock();
    this->mode = mode;
    writeIndex = 0.0;
    buffer->fillBuffer(buffer->getDefaultValue());
    unlock();
}

DSP_FLOAT dsp::BufferRecorder::getExternalBufferSize() const {
    return externalBufferSize;
}

void dsp::BufferRecorder::setExternalBufferSize(DSP_FLOAT externalBufferSize) {
    assert(externalBufferSize >= 0.0);
    lock();
    externalBufferSizeSynced = false;
    setExternalBufferSizeNoLock(externalBufferSize);
    unlock();
}

bool dsp::BufferRecorder::isExternalBufferSizeSynced() const {
    return externalBufferSizeSynced;
}

void dsp::BufferRecorder::setExternalBufferSizeSynced(bool externalBufferSizeSynced) {
    lock();
    if (externalBufferSizeSynced) {
        setExternalBufferSizeNoLock(getBufferSize());
    }
    this->externalBufferSizeSynced = externalBufferSizeSynced;
    unlock();
}

std::shared_ptr<dsp::Buffer> dsp::BufferRecorder::getBuffer() const {
    return buffer;
}

void dsp::BufferRecorder::setBufferSizeNoLock(unsigned int bufferSize) {
    Unit::setBufferSizeNoLock(bufferSize);
    if (externalBufferSizeSynced) {
        setExternalBufferSizeNoLock(bufferSize);
    }
}

void dsp::BufferRecorder::setNumChannelsNoLock(unsigned int numChannels) {
    Unit::setNumChannelsNoLock(numChannels);
    buffer->setNumChannels(numChannels);
    second->setNumChannels(numChannels);
}

void dsp::BufferRecorder::process() {
    Unit::process();
    buffer->lock();
    if (externalBufferSize > 0.0) {
        if (externalBufferSizeSynced || writeIndex >= externalBufferSize) {
            writeIndex = 0.0;
        }
        for (unsigned int i = 0; i < getNumChannels(); i++) {
            std::vector<DSP_FLOAT> &inputBuffer = getInputSignal()->getChannel(i)->getBuffer();
            DSP_FLOAT index = writeIndex;
            for (unsigned int k = 0; k < getBufferSize(); k++) {
                switch (mode) {
                    case Mode::SINGLE_BUFFER: buffer->getChannel(i)[index] = inputBuffer[k]; break;
                    case Mode::DOUBLE_BUFFER: second->getChannel(i)[index] = inputBuffer[k]; break;
                }
                index += 1.0;
                if (index >= externalBufferSize) {
                    if (index < static_cast<unsigned int>(ceil(externalBufferSize))) {
                        switch (mode) {
                            case Mode::SINGLE_BUFFER: buffer->getChannel(i)[index] = inputBuffer[k]; break;
                            case Mode::DOUBLE_BUFFER: second->getChannel(i)[index] = inputBuffer[k]; break;
                        }
                    }
                    index -= externalBufferSize;
                    if (mode == Mode::DOUBLE_BUFFER) {
                        std::copy(second->getChannel(i).begin(),
                                  second->getChannel(i).end(),
                                  buffer->getChannel(i).begin());
                    }
                }
            }
        }
        if (!externalBufferSizeSynced) {
            writeIndex = wrap(writeIndex + getBufferSize(), 0.0, externalBufferSize);
        }
    }
    buffer->unlock();
}

void dsp::BufferRecorder::setExternalBufferSizeNoLock(DSP_FLOAT externalBufferSize) {
    this->externalBufferSize = externalBufferSize;
    unsigned int bufferSize = static_cast<unsigned int>(ceil(externalBufferSize));
    buffer->setBufferSize(bufferSize);
    second->setBufferSize(bufferSize);
}
