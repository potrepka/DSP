#include "TableShaper.h"

dsp::TableShaper::TableShaper(Type outputType, Domain domain)
    : Transformer(Type::RATIO, outputType, domain),
      inputInterpolation(Interpolation::LINEAR),
      positionInterpolation(Interpolation::LINEAR),
      position(std::make_shared<Input>(Type::RATIO, Domain::TIME, 1.0)) {
  getInputs().push_back(position);
}

std::vector<std::shared_ptr<dsp::Buffer>>& dsp::TableShaper::getTables() {
  return tables;
}

dsp::Interpolation dsp::TableShaper::getInputInterpolation() const {
  return inputInterpolation;
}

void dsp::TableShaper::setInputInterpolation(Interpolation interpolation) {
  lock();
  this->inputInterpolation = interpolation;
  unlock();
}

dsp::Interpolation dsp::TableShaper::getPositionInterpolation() const {
  return positionInterpolation;
}

void dsp::TableShaper::setPositionInterpolation(Interpolation interpolation) {
  lock();
  this->positionInterpolation = interpolation;
  unlock();
}

std::shared_ptr<dsp::Input> dsp::TableShaper::getPosition() const {
  return position;
}

void dsp::TableShaper::processNoLock() {
  if (tables.size() > 0) {
    for (const auto& table : tables) {
      if (table != nullptr) {
        table->lock();
      }
    }
    unsigned int numPoints;
    switch (positionInterpolation) {
      case Interpolation::NONE:
        numPoints = 1;
        break;
      case Interpolation::LINEAR:
        numPoints = 2;
        break;
      case Interpolation::HERMITE:
        numPoints = 4;
        break;
    }
    for (size_t channel = 0; channel < getNumChannels(); ++channel) {
      Sample* inputChannel =
          getInput()->getWrapper().getChannelPointer(channel);
      Sample* positionChannel =
          getPosition()->getWrapper().getChannelPointer(channel);
      Sample* outputChannel =
          getOutput()->getWrapper().getChannelPointer(channel);
      for (int sample = 0; sample < getNumSamples(); ++sample) {
        Array samples(numPoints);
        const Sample positionIndex =
            positionChannel[sample] * (tables.size() - 1) +
            (positionInterpolation == Interpolation::NONE ? 0.5 : 0.0);
        const Sample startIndex =
            floor(positionIndex) -
            (positionInterpolation == Interpolation::HERMITE ? 1.0 : 0.0);
        size_t p = static_cast<size_t>(startIndex);
        for (unsigned int j = 0; j < numPoints; ++j) {
          if (p < tables.size() && tables[p] != nullptr) {
            size_t numChannels = tables[p]->getNumChannels();
            size_t numSamples = tables[p]->getNumSamples();
            if (numChannels > 0 && numSamples > 1) {
              size_t c = channel % numChannels;
              Sample* table = tables[p]->getWrapper().getChannelPointer(c);
              Sample clippedInput = clip(inputChannel[sample], -1.0, 1.0);
              Sample index = 0.5 * (clippedInput + 1.0) * (numSamples - 1);
              switch (inputInterpolation) {
                case Interpolation::NONE:
                  samples[j] = table[static_cast<size_t>(index + 0.5)];
                  break;
                case Interpolation::LINEAR:
                  samples[j] = linearClipped(table, numSamples, index,
                                             tables[p]->getChannelValue(c));
                  break;
                case Interpolation::HERMITE:
                  samples[j] = hermiteClipped(table, numSamples, index,
                                              tables[p]->getChannelValue(c));
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
          case Interpolation::NONE:
            outputChannel[sample] = samples[0];
            break;
          case Interpolation::LINEAR:
            outputChannel[sample] = linearClipped(samples.data(), numPoints,
                                                  positionIndex - startIndex);
            break;
          case Interpolation::HERMITE:
            outputChannel[sample] = hermiteClipped(samples.data(), numPoints,
                                                   positionIndex - startIndex);
            break;
        }
      }
    }
    for (const auto& table : tables) {
      if (table != nullptr) {
        table->unlock();
      }
    }
  }
}
