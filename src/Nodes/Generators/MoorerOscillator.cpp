#include "MoorerOscillator.h"

dsp::MoorerOscillator::MoorerOscillator()
        : Producer(Type::RATIO)
        , mode(Mode::ONE_SIDED)
        , phase(std::make_shared<Input>(Type::RATIO))
        , modulationIndex(std::make_shared<Input>(Type::RATIO))
        , intensity(std::make_shared<Input>(Type::LOGARITHMIC))
        , overtones(std::make_shared<Input>(Type::INTEGER)) {
    getInputs().push_back(phase);
    getInputs().push_back(modulationIndex);
    getInputs().push_back(intensity);
    getInputs().push_back(overtones);
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

std::shared_ptr<dsp::Input> dsp::MoorerOscillator::getModulationIndex() const {
    return modulationIndex;
}

std::shared_ptr<dsp::Input> dsp::MoorerOscillator::getIntensity() const {
    return intensity;
}

std::shared_ptr<dsp::Input> dsp::MoorerOscillator::getOvertones() const {
    return overtones;
}

void dsp::MoorerOscillator::processNoLock() {
    for (int channel = 0; channel < getNumChannels(); ++channel) {
        Sample *phaseChannel = getPhase()->getBlock().getChannelPointer(channel);
        Sample *modulationIndexChannel = getModulationIndex()->getBlock().getChannelPointer(channel);
        Sample *intensityChannel = getIntensity()->getBlock().getChannelPointer(channel);
        Sample *overtonesChannel = getOvertones()->getBlock().getChannelPointer(channel);
        Sample *outputChannel = getOutput()->getBlock().getChannelPointer(channel);
        for (int sample = 0; sample < getNumSamples(); ++sample) {
            Sample t = TAU * phaseChannel[sample];
            Sample s = modulationIndexChannel[sample] * t;
            Sample a = exp2(-1.0 / intensityChannel[sample]);
            Sample n = overtonesChannel[sample];
            Sample aa = a * a;
            Sample a1 = a;
            for (unsigned int j = 0; j < n; ++j) {
                a1 *= a;
            }
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
