#include "TableOscillator.h"

dsp::TableOscillator::TableOscillator(Type type)
        : Generator(type)
        , position(pushInput(Type::UNIPOLAR))
        , phase(pushInput(Type::UNIPOLAR))
        , positionInterpolation(Interpolation::LINEAR)
        , phaseInterpolation(Interpolation::LINEAR) {}

dsp::Interpolation dsp::TableOscillator::getPositionInterpolation() const {
    return positionInterpolation;
}

void dsp::TableOscillator::setPositionInterpolation(Interpolation interpolation) {
    lock();
    positionInterpolation = interpolation;
    unlock();
}

dsp::Interpolation dsp::TableOscillator::getPhaseInterpolation() const {
    return phaseInterpolation;
}

void dsp::TableOscillator::setPhaseInterpolation(Interpolation interpolation) {
    lock();
    phaseInterpolation = interpolation;
    unlock();
}

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
        int numPoints;
        switch (positionInterpolation) {
            case Interpolation::NONE: numPoints = 1; break;
            case Interpolation::LINEAR: numPoints = 2; break;
            case Interpolation::HERMITE: numPoints = 4; break;
        }
#if DSP_USE_VC
        std::vector<Array> samples(Vector::Size, Array(numPoints));
#else
        Array samples(numPoints);
#endif
        for (unsigned int i = 0; i < getNumChannels(); ++i) {
            Array &positionBuffer = getPosition()->getChannel(i)->getBuffer();
            Array &phaseBuffer = getPhase()->getChannel(i)->getBuffer();
            Array &outputBuffer = getOutputSignal()->getChannel(i)->getBuffer();
            Iterator phaseIterator = phaseBuffer.begin();
            Iterator positionIterator = positionBuffer.begin();
            Iterator outputIterator = outputBuffer.begin();
            while (outputIterator != outputBuffer.end()) {
#if DSP_USE_VC
                const Vector positionIndex =
                        clip(*positionIterator, 0.0, 1.0) *
                        (collection.size() - (positionInterpolation == Interpolation::NONE ? 0 : 1));
                const Vector indexBefore =
                        Vc::floor(positionIndex) -
                        (positionInterpolation == Interpolation::HERMITE ? Vector::One() : Vector::Zero());
                Vector p = indexBefore;
#else
                const Sample positionIndex =
                        clip(*positionIterator, 0.0, 1.0) *
                        (collection.size() - (positionInterpolation == Interpolation::NONE ? 0 : 1));
                const Sample indexBefore =
                        floor(positionIndex) - (positionInterpolation == Interpolation::HERMITE ? 1.0 : 0.0);
                Sample p = indexBefore;
#endif
                for (int j = 0; j < numPoints; ++j) {
#if DSP_USE_VC
                    for (int k = 0; k < Vector::Size; ++k) {
                        if (p[k] >= 0.0 && p[k] < collection.size() && collection[p[k]] != nullptr) {
                            if (collection[p[k]]->getNumChannels() > 0) {
                                Array &buffer = collection[p[k]]->getChannel(i % collection[p[k]]->getNumChannels());
                                Sample index = wrap((*phaseIterator)[k], 1.0) * collection[p[k]]->getBufferSize();
                                switch (phaseInterpolation) {
                                    case Interpolation::NONE: samples[k][j] = buffer[index]; break;
                                    case Interpolation::LINEAR:
                                        samples[k][j] = linear(buffer, index, collection[p[k]]->getDefaultValue());
                                        break;
                                    case Interpolation::HERMITE:
                                        samples[k][j] = hermite(buffer, index, collection[p[k]]->getDefaultValue());
                                        break;
                                }
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
                            Array &buffer = collection[p]->getChannel(i % collection[p]->getNumChannels());
                            Sample index = wrap(*phaseIterator, 1.0) * collection[p]->getBufferSize();
                            switch (phaseInterpolation) {
                                case Interpolation::NONE: samples[j] = buffer[index]; break;
                                case Interpolation::LINEAR:
                                    samples[j] = linear(buffer, index, collection[p]->getDefaultValue());
                                    break;
                                case Interpolation::HERMITE:
                                    samples[j] = hermite(buffer, index, collection[p]->getDefaultValue());
                                    break;
                            }
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
                    switch (positionInterpolation) {
                        case Interpolation::NONE: outputVector[k] = samples[k][0]; break;
                        case Interpolation::LINEAR:
                            outputVector[k] = linear(samples[k], positionIndex[k] - indexBefore[k]);
                            break;
                        case Interpolation::HERMITE:
                            outputVector[k] = hermite(samples[k], positionIndex[k] - indexBefore[k]);
                            break;
                    }
                }
                *outputIterator = outputVector;
#else
                switch (positionInterpolation) {
                    case Interpolation::NONE: *outputIterator = samples[0]; break;
                    case Interpolation::LINEAR: *outputIterator = linear(samples, positionIndex - indexBefore); break;
                    case Interpolation::HERMITE: *outputIterator = hermite(samples, positionIndex - indexBefore); break;
                }
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
