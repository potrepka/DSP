#include "BufferOscillator.h"

const unsigned int dsp::BufferOscillator::PHASE = 0;
const unsigned int dsp::BufferOscillator::POSITION = 1;

dsp::BufferOscillator::BufferOscillator() : Generator(Type::BIPOLAR) {
    pushInput(Type::UNIPOLAR);
    pushInput(Type::UNIPOLAR);
}

unsigned int dsp::BufferOscillator::getNumBuffers() {
    return static_cast<unsigned int>(buffers.size());
}

std::shared_ptr<dsp::Buffer> dsp::BufferOscillator::getBuffer(unsigned int index) {
    return buffers[index];
}

void dsp::BufferOscillator::setBuffer(unsigned int index, std::shared_ptr<Buffer> buffer) {
    assert(buffer != nullptr);
    lock();
    buffers[index] = buffer;
    unlock();
}

void dsp::BufferOscillator::pushBuffer(std::shared_ptr<Buffer> buffer) {
    assert(buffer != nullptr);
    lock();
    buffers.push_back(buffer);
    unlock();
}

void dsp::BufferOscillator::insertBuffer(unsigned int index, std::shared_ptr<Buffer> buffer) {
    assert(buffer != nullptr);
    lock();
    buffers.insert(buffers.begin() + index, buffer);
    unlock();
}

void dsp::BufferOscillator::removeBuffer(unsigned int index) {
    lock();
    buffers.erase(buffers.begin() + index);
    unlock();
}

std::shared_ptr<dsp::Unit::InputParameter> dsp::BufferOscillator::getPhase() {
    return getInput(PHASE);
}

std::shared_ptr<dsp::Unit::InputParameter> dsp::BufferOscillator::getPosition() {
    return getInput(POSITION);
}

void dsp::BufferOscillator::process() {
    Unit::process();
    if (buffers.size() > 0) {
        for (const auto &buffer : buffers) {
            buffer->lock();
        }
        std::vector<DSP_FLOAT> points;
        points.resize(4);
        for (unsigned int i = 0; i < getNumChannels(); i++) {
            std::vector<DSP_FLOAT> &outputBuffer = getOutputSignal()->getChannel(i)->getBuffer();
            std::vector<DSP_FLOAT> &phaseBuffer = getPhase()->getChannel(i)->getBuffer();
            std::vector<DSP_FLOAT> &positionBuffer = getPosition()->getChannel(i)->getBuffer();
            for (unsigned int k = 0; k < getBufferSize(); k++) {
                const DSP_FLOAT positionIndex = clip(positionBuffer[k], 0.0, 1.0) * (buffers.size() - 1);
                const long indexBefore = static_cast<long>(positionIndex) - 1;
                long p = indexBefore;
                for (unsigned char j = 0; j < 4; j++) {
                    if (p >= 0 && p < buffers.size() && buffers[p]->getNumChannels() > 0) {
                        points[j] = linear(buffers[p]->getChannel(i % buffers[p]->getNumChannels()),
                                           wrap(phaseBuffer[k], 0.0, 1.0) * buffers[p]->getBufferSize(),
                                           buffers[p]->getDefaultValue());
                    } else {
                        points[j] = 0.0;
                    }
                    p++;
                }
                outputBuffer[k] = hermite(points, positionIndex - indexBefore);
            }
        }
        for (const auto &buffer : buffers) {
            buffer->unlock();
        }
    }
}
