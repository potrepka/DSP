#include "Clipper.h"

dsp::Clipper::Clipper(Type type, Space space)
    : Transformer(type, space),
      mode(Mode::CLIP),
      min(std::make_shared<Input>(type, space, 0.0, -1.0)),
      max(std::make_shared<Input>(type, space, 0.0, 1.0)) {
  getInputs().push_back(min);
  getInputs().push_back(max);
}

dsp::Clipper::Mode dsp::Clipper::getMode() const { return mode; }

void dsp::Clipper::setMode(Mode mode) {
  lock();
  this->mode = mode;
  unlock();
}

std::shared_ptr<dsp::Input> dsp::Clipper::getMin() const { return min; }

std::shared_ptr<dsp::Input> dsp::Clipper::getMax() const { return max; }

void dsp::Clipper::processNoLock() {
  for (size_t channel = 0; channel < getNumChannels(); ++channel) {
    Sample* inputChannel = getInput()->getWrapper().getChannelPointer(channel);
    Sample* minChannel = getMin()->getWrapper().getChannelPointer(channel);
    Sample* maxChannel = getMax()->getWrapper().getChannelPointer(channel);
    Sample* outputChannel =
        getOutput()->getWrapper().getChannelPointer(channel);
    for (size_t sample = 0; sample < getNumSamples(); ++sample) {
      Sample& input = inputChannel[sample];
      Sample& min = minChannel[sample];
      Sample& max = maxChannel[sample];
      Sample& output = outputChannel[sample];
      switch (mode) {
        case Mode::CLIP:
          output = clip(input, min, max);
          break;
        case Mode::WRAP:
          output = wrap(input - min, max - min) + min;
          break;
        case Mode::FOLD: {
          Sample offset = input - min;
          Sample range = max - min;
          Sample wrapped = wrap(offset, range);
          Sample flipped =
              wrap(offset, 2.0 * range) < range ? wrapped : range - wrapped;
          output = flipped + min;
        } break;
      }
    }
  }
}
