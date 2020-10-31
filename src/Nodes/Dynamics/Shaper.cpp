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
            if (drive == 0.0) {
                output = 0.0;
            } else if (drive == 1.0) {
                output = input;
            } else {
                const Sample modeClipped = clip(mode, Mode::MIN, Mode::MAX);
                switch (static_cast<int>(modeClipped)) {
                    case Mode::POLYNOMIAL: {
                        Sample onePlusInput = 1.0 + input;
                        Sample oneMinusInput = 1.0 - input;
                        Sample driveMinusOne = drive - 1.0;
                        output = input < 0.0 ? onePlusInput * pow(abs(onePlusInput), driveMinusOne) - 1.0
                                             : 1.0 - oneMinusInput * pow(oneMinusInput, driveMinusOne);
                    } break;
                    case Mode::HYPERBOLIC: {
                        Sample driveMinusOne = drive - 1.0;
                        output = input < 0.0 ? (drive / (1.0 - driveMinusOne * input) - drive) / driveMinusOne
                                             : (drive - drive / (1.0 + driveMinusOne * input)) / driveMinusOne;
                    } break;
                }
            }
        }
    }
}
