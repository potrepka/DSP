#include "MoorerOscillator.h"

dsp::MoorerOscillator::MoorerOscillator()
        : Producer(Type::RATIO)
        , phase(std::make_shared<Input>(Type::RATIO))
        , intensity(std::make_shared<Input>(Type::RATIO, Space::TIME, 1.0))
        , modulationIndex(std::make_shared<Input>(Type::RATIO))
        , harmonics(std::make_shared<Input>(Type::INTEGER))
        , mode(std::make_shared<Input>(Type::INTEGER, Space::TIME, Mode::MAX)) {
    getInputs().push_back(phase);
    getInputs().push_back(intensity);
    getInputs().push_back(modulationIndex);
    getInputs().push_back(harmonics);
    getInputs().push_back(mode);
}

std::shared_ptr<dsp::Input> dsp::MoorerOscillator::getPhase() const {
    return phase;
}

std::shared_ptr<dsp::Input> dsp::MoorerOscillator::getIntensity() const {
    return intensity;
}

std::shared_ptr<dsp::Input> dsp::MoorerOscillator::getModulationIndex() const {
    return modulationIndex;
}

std::shared_ptr<dsp::Input> dsp::MoorerOscillator::getHarmonics() const {
    return harmonics;
}

std::shared_ptr<dsp::Input> dsp::MoorerOscillator::getMode() const {
    return mode;
}

void dsp::MoorerOscillator::processNoLock() {
    for (int channel = 0; channel < getNumChannels(); ++channel) {
        Sample *phaseChannel = getPhase()->getWrapper().getChannelPointer(channel);
        Sample *intensityChannel = getIntensity()->getWrapper().getChannelPointer(channel);
        Sample *modulationIndexChannel = getModulationIndex()->getWrapper().getChannelPointer(channel);
        Sample *harmonicsChannel = getHarmonics()->getWrapper().getChannelPointer(channel);
        Sample *modeChannel = getMode()->getWrapper().getChannelPointer(channel);
        Sample *outputChannel = getOutput()->getWrapper().getChannelPointer(channel);
        for (int sample = 0; sample < getNumSamples(); ++sample) {
            const Sample a = intensityChannel[sample];
            const Sample n = harmonicsChannel[sample] - 1.0;
            if (a == 1.0 || n < 0.0) {
                outputChannel[sample] = 0.0;
            } else {
                Sample &mode = modeChannel[sample];
                const Sample t = TAU * phaseChannel[sample];
                const Sample s = modulationIndexChannel[sample] * t;
                const size_t nFloor = static_cast<size_t>(n);
                const Sample aa = a * a;
                Sample a1 = a;
                for (size_t j = 0; j < nFloor / 2; ++j) {
                    a1 *= aa;
                }
                a1 *= nFloor & 1 ? a : 1;
                Sample scale;
                Sample numerator;
                switch (static_cast<int>(mode)) {
                    case Mode::ONE_SIDED:
                        scale = (a - 1.0) / (a1 - 1.0);
                        numerator = sin(t) - a * sin(t - s) - a1 * (sin(t + (n + 1.0) * s) - a * sin(t + n * s));
                        break;
                    case Mode::TWO_SIDED:
                        scale = (a - 1.0) / (a1 + a1 - a - 1.0);
                        numerator = sin(t) * (1.0 - aa - (a1 + a1) * (cos((n + 1.0) * s) - a * cos(n * s)));
                        break;
                }
                const Sample denominator = 1.0 + aa - (a + a) * cos(s);
                outputChannel[sample] = scale * numerator / denominator;
            }
        }
    }
}
