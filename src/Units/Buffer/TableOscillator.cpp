#include "TableOscillator.h"

dsp::TableOscillator::TableOscillator(Type type)
        : Generator(type)
        , phase(pushInput(Type::UNIPOLAR))
        , position(pushInput(Type::UNIPOLAR)) {}

unsigned int dsp::TableOscillator::getNumTables() const {
    return getNumBuffers();
}

std::shared_ptr<dsp::Buffer> dsp::TableOscillator::getTable(unsigned int index) const {
    return getBuffer(index);
}

std::vector<std::shared_ptr<dsp::Buffer>> dsp::TableOscillator::getTables(unsigned int begin, unsigned int end) const {
    return getBuffers(begin, end);
}

void dsp::TableOscillator::pushTable(std::shared_ptr<Buffer> table) {
    lock();
    pushBuffer(table);
    unlock();
}

void dsp::TableOscillator::pushTables(std::vector<std::shared_ptr<Buffer>> tables) {
    lock();
    pushBuffers(tables);
    unlock();
}

void dsp::TableOscillator::replaceTable(std::shared_ptr<Buffer> table, std::shared_ptr<Buffer> replacement) {
    lock();
    replaceBuffer(table, replacement);
    unlock();
}

void dsp::TableOscillator::removeTable(std::shared_ptr<Buffer> table) {
    lock();
    removeBuffer(table);
    unlock();
}

void dsp::TableOscillator::removeTables(std::vector<std::shared_ptr<Buffer>> tables) {
    lock();
    removeBuffers(tables);
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
    if (collection.size() > 0) {
        for (const auto &buffer : collection) {
            if (buffer != nullptr) {
                buffer->lock();
            }
        }
        Array points(4);
        for (unsigned int i = 0; i < getNumChannels(); i++) {
            Array &phaseBuffer = getPhase()->getChannel(i)->getBuffer();
            Array &positionBuffer = getPosition()->getChannel(i)->getBuffer();
            Array &outputBuffer = getOutputSignal()->getChannel(i)->getBuffer();
            for (unsigned int k = 0; k < getBufferSize(); k++) {
                const Sample positionIndex = clip(positionBuffer[k], 0.0, 1.0) * (collection.size() - 1);
                const long indexBefore = static_cast<long>(positionIndex) - 1;
                long p = indexBefore;
                for (unsigned char j = 0; j < 4; j++) {
                    if (p >= 0 && p < collection.size() && collection[p] != nullptr) {
                        if (collection[p]->getNumChannels() > 0 && collection[p]->getBufferSize() > 0) {
                            points[j] = linear(collection[p]->getChannel(i % collection[p]->getNumChannels()),
                                               wrap(phaseBuffer[k], 0.0, 1.0) * collection[p]->getBufferSize(),
                                               collection[p]->getDefaultValue());
                        } else {
                            points[j] = collection[p]->getDefaultValue();
                        }
                    } else {
                        points[j] = getOutputSignal()->getDefaultValue();
                    }
                    p++;
                }
                outputBuffer[k] = hermite(points, positionIndex - indexBefore);
            }
        }
        for (const auto &buffer : collection) {
            if (buffer != nullptr) {
                buffer->unlock();
            }
        }
    }
}
