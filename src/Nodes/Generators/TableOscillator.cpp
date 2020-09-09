#include "TableOscillator.h"

dsp::TableOscillator::TableOscillator(Type type)
        : Producer(type)
        , positionInterpolation(Interpolation::LINEAR)
        , phaseInterpolation(Interpolation::LINEAR)
        , position(std::make_shared<Input>(Type::RATIO))
        , phase(std::make_shared<Input>(Type::RATIO)) {
    getInputs().push_back(position);
    getInputs().push_back(phase);
}

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

std::vector<std::shared_ptr<dsp::Buffer>> &dsp::TableOscillator::getTables() {
    return tables;
}

std::shared_ptr<dsp::Input> dsp::TableOscillator::getPosition() const {
    return position;
}

std::shared_ptr<dsp::Input> dsp::TableOscillator::getPhase() const {
    return phase;
}

void dsp::TableOscillator::processNoLock() {
    if (tables.size() > 0) {
        for (const auto &table : tables) {
            if (table != nullptr) {
                table->lock();
            }
        }
        int numPoints;
        switch (positionInterpolation) {
            case Interpolation::NONE: numPoints = 1; break;
            case Interpolation::LINEAR: numPoints = 2; break;
            case Interpolation::HERMITE: numPoints = 4; break;
        }
        Array samples(numPoints);
        for (int channel = 0; channel < getNumChannels(); ++channel) {
            Sample *phaseChannel = getPhase()->getBlock().getChannelPointer(channel);
            Sample *positionChannel = getPosition()->getBlock().getChannelPointer(channel);
            Sample *outputChannel = getOutput()->getBlock().getChannelPointer(channel);
            for (int sample = 0; sample < getNumSamples(); ++sample) {
                const Sample positionIndex = clip(positionChannel[sample], 0.0, 1.0) *
                                             (tables.size() - (positionInterpolation == Interpolation::NONE ? 0 : 1));
                const Sample indexBefore =
                        floor(positionIndex) - (positionInterpolation == Interpolation::HERMITE ? 1.0 : 0.0);
                Sample p = indexBefore;
                for (int j = 0; j < numPoints; ++j) {
                    if (p >= 0 && p < tables.size() && tables[p] != nullptr) {
                        int numChannels = tables[p]->getNumChannels();
                        int numSamples = tables[p]->getNumSamples();
                        if (numChannels > 0) {
                            int c = channel % numChannels;
                            Sample *table = tables[p]->getBlock().getChannelPointer(c);
                            Sample index = wrap(phaseChannel[sample], 1.0) * numSamples;
                            switch (phaseInterpolation) {
                                case Interpolation::NONE: samples[j] = table[static_cast<int>(index)]; break;
                                case Interpolation::LINEAR:
                                    samples[j] = linear(table, numSamples, index, tables[p]->getChannelValue(c));
                                    break;
                                case Interpolation::HERMITE:
                                    samples[j] = hermite(table, numSamples, index, tables[p]->getChannelValue(c));
                                    break;
                            }
                        } else {
                            samples[j] = tables[p]->getDefaultValue();
                        }
                    } else {
                        samples[j] = getOutput()->getChannelValue(channel);
                    }
                    ++p;
                }
                switch (positionInterpolation) {
                    case Interpolation::NONE: outputChannel[sample] = samples[0]; break;
                    case Interpolation::LINEAR:
                        outputChannel[sample] = linear(samples.data(), numPoints, positionIndex - indexBefore);
                        break;
                    case Interpolation::HERMITE:
                        outputChannel[sample] = hermite(samples.data(), numPoints, positionIndex - indexBefore);
                        break;
                }
            }
        }
        for (const auto &table : tables) {
            if (table != nullptr) {
                table->unlock();
            }
        }
    }
}
