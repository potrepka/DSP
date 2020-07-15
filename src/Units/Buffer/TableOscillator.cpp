#include "TableOscillator.h"

dsp::TableOscillator::TableOscillator(Type type)
        : Generator(type)
        , phase(pushInput(Type::UNIPOLAR))
        , position(pushInput(Type::UNIPOLAR)) {}

unsigned int dsp::TableOscillator::getNumTables() const {
    return static_cast<unsigned int>(buffers.size());
}

std::shared_ptr<dsp::Buffer> dsp::TableOscillator::getTable(unsigned int index) const {
    assert(index < buffers.size());
    return buffers[index];
}

std::vector<std::shared_ptr<dsp::Buffer>> dsp::TableOscillator::getTables(unsigned int begin, unsigned int end) const {
    assert(begin <= end && end <= buffers.size());
    return std::vector<std::shared_ptr<dsp::Buffer>>(buffers.begin() + begin, buffers.begin() + end);
}

void dsp::TableOscillator::pushTable(std::shared_ptr<Buffer> table) {
    lock();
    buffers.push_back(table);
    unlock();
}

void dsp::TableOscillator::pushTables(std::vector<std::shared_ptr<Buffer>> tables) {
    lock();
    for (const auto &table : tables) {
        buffers.push_back(table);
    }
    unlock();
}

void dsp::TableOscillator::removeTable(std::shared_ptr<Buffer> table) {
    lock();
    buffers.erase(std::remove(buffers.begin(), buffers.end(), table), buffers.end());
    unlock();
}

void dsp::TableOscillator::removeTables(std::vector<std::shared_ptr<Buffer>> tables) {
    lock();
    for (const auto &table : tables) {
        buffers.erase(std::remove(buffers.begin(), buffers.end(), table), buffers.end());
    }
    unlock();
}

std::shared_ptr<dsp::InputParameter> dsp::TableOscillator::getPhase() const {
    return phase;
}

std::shared_ptr<dsp::InputParameter> dsp::TableOscillator::getPosition() const {
    return position;
}

void dsp::TableOscillator::process() {
    Unit::process();
    if (buffers.size() > 0) {
        for (const auto &buffer : buffers) {
            if (buffer != nullptr) {
                buffer->lock();
            }
        }
        std::vector<DSP_FLOAT> points(4);
        for (unsigned int i = 0; i < getNumChannels(); i++) {
            std::vector<DSP_FLOAT> &phaseBuffer = getPhase()->getChannel(i)->getBuffer();
            std::vector<DSP_FLOAT> &positionBuffer = getPosition()->getChannel(i)->getBuffer();
            std::vector<DSP_FLOAT> &outputBuffer = getOutputSignal()->getChannel(i)->getBuffer();
            for (unsigned int k = 0; k < getBufferSize(); k++) {
                const DSP_FLOAT positionIndex = clip(positionBuffer[k], 0.0, 1.0) * (buffers.size() - 1);
                const long indexBefore = static_cast<long>(positionIndex) - 1;
                long p = indexBefore;
                for (unsigned char j = 0; j < 4; j++) {
                    if (p >= 0 && p < buffers.size() && buffers[p] != nullptr && buffers[p]->getNumChannels() > 0) {
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
            if (buffer != nullptr) {
                buffer->unlock();
            }
        }
    }
}
