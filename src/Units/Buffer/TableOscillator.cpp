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
#if DSP_USE_VC
        std::vector<Array> samples(Vector::Size, Array(4));
#else
        Array samples(4);
#endif
        for (unsigned int i = 0; i < getNumChannels(); ++i) {
            Array &phaseBuffer = getPhase()->getChannel(i)->getBuffer();
            Array &positionBuffer = getPosition()->getChannel(i)->getBuffer();
            Array &outputBuffer = getOutputSignal()->getChannel(i)->getBuffer();
            Iterator phaseIterator = phaseBuffer.begin();
            Iterator positionIterator = positionBuffer.begin();
            Iterator outputIterator = outputBuffer.begin();
            while (outputIterator != outputBuffer.end()) {
#if DSP_USE_VC
                const Vector positionIndex = clip(*positionIterator, 0.0, 1.0) * (collection.size() - 1);
                const Vector indexBefore = Vc::floor(positionIndex) - 1.0;
                Vector p = indexBefore;
#else
                const Sample positionIndex = clip(*positionIterator, 0.0, 1.0) * (collection.size() - 1);
                const Sample indexBefore = floor(positionIndex) - 1.0;
                Sample p = indexBefore;
#endif
                for (unsigned char j = 0; j < 4; ++j) {
#if DSP_USE_VC
                    for (int k = 0; k < Vector::Size; ++k) {
                        if (p[k] >= 0.0 && p[k] < collection.size() && collection[p[k]] != nullptr) {
                            if (collection[p[k]]->getNumChannels() > 0) {
                                samples[k][j] =
                                        linear(collection[p[k]]->getChannel(i % collection[p[k]]->getNumChannels()),
                                               wrap((*phaseIterator)[k], 1.0) * collection[p[k]]->getBufferSize(),
                                               collection[p[k]]->getDefaultValue());
                            } else {
                                samples[k][j] = collection[p[k]]->getDefaultValue();
                            }
                        } else {
                            samples[k][j] = getOutputSignal()->getDefaultValue();
                        }
                    }
#else
                    if (p >= 0 && p < collection.size() && collection[p] != nullptr) {
                        if (collection[p]->getNumChannels() > 0) {
                            samples[j] = linear(collection[p]->getChannel(i % collection[p]->getNumChannels()),
                                                wrap(*phaseIterator, 1.0) * collection[p]->getBufferSize(),
                                                collection[p]->getDefaultValue());
                        } else {
                            samples[j] = collection[p]->getDefaultValue();
                        }
                    } else {
                        samples[j] = getOutputSignal()->getDefaultValue();
                    }
#endif
                    ++p;
                }
#if DSP_USE_VC
                Vector outputVector;
                for (int k = 0; k < Vector::Size; ++k) {
                    outputVector[k] = hermite(samples[k], positionIndex[k] - indexBefore[k]);
                }
                *outputIterator = outputVector;
#else
                *outputIterator = hermite(samples, positionIndex - indexBefore);
#endif
                ++phaseIterator;
                ++positionIterator;
                ++outputIterator;
            }
        }
        for (const auto &buffer : collection) {
            if (buffer != nullptr) {
                buffer->unlock();
            }
        }
    }
}
