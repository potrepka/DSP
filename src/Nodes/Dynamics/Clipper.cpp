#include "Clipper.h"

dsp::Clipper::Clipper(Type type, Space space)
        : Transformer(type, type, space)
        , minimum(std::make_shared<Input>(type, space, 0.0, -1.0))
        , maximum(std::make_shared<Input>(type, space, 0.0, 1.0))
        , mode(std::make_shared<Input>(Type::INTEGER, space, Mode::MAX)) {
    getInputs().push_back(minimum);
    getInputs().push_back(maximum);
    getInputs().push_back(mode);
}

std::shared_ptr<dsp::Input> dsp::Clipper::getMinimum() const {
    return minimum;
}

std::shared_ptr<dsp::Input> dsp::Clipper::getMaximum() const {
    return maximum;
}

std::shared_ptr<dsp::Input> dsp::Clipper::getMode() const {
    return mode;
}

void dsp::Clipper::processNoLock() {
    for (size_t channel = 0; channel < getNumChannels(); ++channel) {
        Sample *inputChannel = getInput()->getWrapper().getChannelPointer(channel);
        Sample *minimumChannel = getMinimum()->getWrapper().getChannelPointer(channel);
        Sample *maximumChannel = getMaximum()->getWrapper().getChannelPointer(channel);
        Sample *modeChannel = getMode()->getWrapper().getChannelPointer(channel);
        Sample *outputChannel = getOutput()->getWrapper().getChannelPointer(channel);
        for (size_t sample = 0; sample < getNumSamples(); ++sample) {
            Sample &input = inputChannel[sample];
            Sample &minimum = minimumChannel[sample];
            Sample &maximum = maximumChannel[sample];
            Sample &mode = modeChannel[sample];
            Sample &output = outputChannel[sample];
            switch (static_cast<int>(mode)) {
                case Mode::CLIP: output = clip(input, minimum, maximum); break;
                case Mode::WRAP: output = wrap(input - minimum, maximum - minimum) + minimum; break;
                case Mode::MIRROR: {
                    Sample offset = input - minimum;
                    Sample range = maximum - minimum;
                    Sample wrapped = wrap(offset, range);
                    Sample flipped = wrap(offset, 2.0 * range) < range ? wrapped : range - wrapped;
                    output = flipped + minimum;
                } break;
            }
        }
    }
}
