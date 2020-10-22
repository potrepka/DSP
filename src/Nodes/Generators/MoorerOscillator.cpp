#include "MoorerOscillator.h"

dsp::MoorerOscillator::MoorerOscillator()
        : Producer(Type::RATIO)
        , mode(Mode::ONE_SIDED)
        , phase(std::make_shared<Input>(Type::RATIO))
        , intensity(std::make_shared<Input>(Type::RATIO))
        , modulationIndex(std::make_shared<Input>(Type::RATIO))
        , harmonics(std::make_shared<Input>(Type::INTEGER)) {
    getInputs().push_back(phase);
    getInputs().push_back(intensity);
    getInputs().push_back(modulationIndex);
    getInputs().push_back(harmonics);
}

dsp::MoorerOscillator::Mode dsp::MoorerOscillator::getMode() const {
    return mode;
}

void dsp::MoorerOscillator::setMode(Mode mode) {
    lock();
    this->mode = mode;
    unlock();
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

void dsp::MoorerOscillator::processNoLock() {
    for (int channel = 0; channel < getNumChannels(); ++channel) {
        Sample *phaseChannel = getPhase()->getWrapper().getChannelPointer(channel);
        Sample *intensityChannel = getIntensity()->getWrapper().getChannelPointer(channel);
        Sample *modulationIndexChannel = getModulationIndex()->getWrapper().getChannelPointer(channel);
        Sample *harmonicsChannel = getHarmonics()->getWrapper().getChannelPointer(channel);
        Sample *outputChannel = getOutput()->getWrapper().getChannelPointer(channel);
        for (int sample = 0; sample < getNumSamples(); ++sample) {
            Sample a = clip(intensityChannel[sample], 0.0, 1.0);
            Sample n = harmonicsChannel[sample] - 1.0;
            if (a == 1.0 || n < 0.0) {
                outputChannel[sample] = 0.0;
            } else {
                Sample t = TAU * phaseChannel[sample];
                Sample s = modulationIndexChannel[sample] * t;
                size_t nFloor = static_cast<size_t>(n);
                Sample aa = a * a;
                Sample a1 = a;
                for (size_t j = 0; j < nFloor / 2; ++j) {
                    a1 *= aa;
                }
                a1 *= nFloor & 1 ? a : 1;
                Sample scale;
                Sample numerator;
                switch (mode) {
                    case Mode::ONE_SIDED:
                        scale = (a - 1.0) / (a1 - 1.0);
                        numerator = sin(t) - a * sin(t - s) - a1 * (sin(t + (n + 1.0) * s) - a * sin(t + n * s));
                        break;
                    case Mode::TWO_SIDED:
                        scale = (a - 1.0) / (a1 + a1 - a - 1.0);
                        numerator = sin(t) * (1.0 - aa - (a1 + a1) * (cos((n + 1.0) * s) - a * cos(n * s)));
                        break;
                }
                Sample denominator = 1.0 + aa - (a + a) * cos(s);
                outputChannel[sample] = scale * numerator / denominator;
            }
        }
    }
}
