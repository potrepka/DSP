#include "Shaper.h"

dsp::Shaper::Shaper(Space space)
        : Transformer(Type::RATIO, Type::RATIO, space)
        , drive(std::make_shared<Input>(Type::RATIO, space, 0.0, 1.0))
        , mode(std::make_shared<Input>(Type::INTEGER, space, Mode::MAX)) {
    getInputs().push_back(drive);
    getInputs().push_back(mode);
}

std::shared_ptr<dsp::Input> dsp::Shaper::getDrive() const {
    return drive;
}

std::shared_ptr<dsp::Input> dsp::Shaper::getMode() const {
    return mode;
}

dsp::Sample dsp::Shaper::getOutputSignal(size_t channel, Sample input) {
    lock();
    DSP_ASSERT(channel < getNumChannels());
    if (getNumSamples() > 0) {
        const size_t lastSample = getNumSamples() - 1;
        const Sample drive = getDrive()->getWrapper().getSample(channel, lastSample);
        const Sample mode = getMode()->getWrapper().getSample(channel, lastSample);
        unlock();
        return getOutputSignal(input, drive, mode);
    } else {
        unlock();
        return input;
    }
}

void dsp::Shaper::processNoLock() {
    for (size_t channel = 0; channel < getNumChannels(); ++channel) {
        Sample *inputChannel = getInput()->getWrapper().getChannelPointer(channel);
        Sample *driveChannel = getDrive()->getWrapper().getChannelPointer(channel);
        Sample *modeChannel = getMode()->getWrapper().getChannelPointer(channel);
        Sample *outputChannel = getOutput()->getWrapper().getChannelPointer(channel);
        for (size_t sample = 0; sample < getNumSamples(); ++sample) {
            Sample &input = inputChannel[sample];
            Sample &drive = driveChannel[sample];
            Sample &mode = modeChannel[sample];
            Sample &output = outputChannel[sample];
            output = getOutputSignal(input, drive, mode);
        }
    }
}

dsp::Sample dsp::Shaper::getOutputSignal(const Sample &input, const Sample &drive, const Sample &mode) {
    bool driveIsNegative = drive < 0.0;
    Sample clipped = clip(driveIsNegative ? -input : input, -1.0, 1.0);
    if (drive == 0.0) {
        return 0.0;
    } else if (drive == 1.0) {
        return clipped;
    } else {
        Sample posDrive = driveIsNegative ? -drive : drive;
        switch (static_cast<int>(mode)) {
            case Mode::HYPERBOLIC: {
                if (posDrive > 1.0) {
                    Sample driveMinusOne = posDrive - 1.0;
                    return tanh(clipped * driveMinusOne) / tanh(driveMinusOne);
                } else {
                    Sample driveMinusOne = 1.0 / posDrive - 1.0;
                    return atanh(clipped * tanh(driveMinusOne)) / driveMinusOne;
                }
            }
            case Mode::RATIONAL: {
                Sample driveMinusOne = posDrive - 1.0;
                return clipped < 0.0 ? (posDrive / (1.0 - driveMinusOne * clipped) - posDrive) / driveMinusOne
                                     : (posDrive - posDrive / (1.0 + driveMinusOne * clipped)) / driveMinusOne;
            }
        }
    }
}
