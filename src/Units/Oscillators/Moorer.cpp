#include "Moorer.h"

dsp::Moorer::Moorer()
        : Generator(Type::BIPOLAR)
        , phase(pushInput(Type::UNIPOLAR))
        , modulationRatio(pushInput(Type::RATIO, Space::TIME, 1.0))
        , intensity(pushInput(Type::UNIPOLAR))
        , overtones(pushInput(Type::INTEGER))
        , mode(Mode::ONE_SIDED) {}

dsp::Moorer::Mode dsp::Moorer::getMode() {
    return mode;
}

void dsp::Moorer::setMode(Mode mode) {
    lock();
    this->mode = mode;
    unlock();
}

std::shared_ptr<dsp::InputParameter> dsp::Moorer::getPhase() const {
    return phase;
}

std::shared_ptr<dsp::InputParameter> dsp::Moorer::getModulationRatio() const {
    return modulationRatio;
}

std::shared_ptr<dsp::InputParameter> dsp::Moorer::getIntensity() const {
    return intensity;
}

std::shared_ptr<dsp::InputParameter> dsp::Moorer::getOvertones() const {
    return overtones;
}

void dsp::Moorer::process() {
    Unit::process();
    for (unsigned int i = 0; i < getNumChannels(); ++i) {
        Array &phaseBuffer = getPhase()->getChannel(i)->getBuffer();
        Array &modulationRatioBuffer = getModulationRatio()->getChannel(i)->getBuffer();
        Array &intensityBuffer = getIntensity()->getChannel(i)->getBuffer();
        Array &overtonesBuffer = getOvertones()->getChannel(i)->getBuffer();
        Array &outputBuffer = getOutputSignal()->getChannel(i)->getBuffer();
        Iterator phaseIterator = phaseBuffer.begin();
        Iterator modulationRatioIterator = modulationRatioBuffer.begin();
        Iterator intensityIterator = intensityBuffer.begin();
        Iterator overtonesIterator = overtonesBuffer.begin();
        Iterator outputIterator = outputBuffer.begin();
        while (outputIterator != outputBuffer.end()) {
#if DSP_USE_VC
            Vector t = TAU * *phaseIterator;
            Vector s = *modulationRatioIterator * t;
            Vector a = *intensityIterator;
            Vector n = *overtonesIterator;
            Vector aa = a * a;
            Vector a1 = a;
            for (unsigned int k = 0; k < Vector::Size; ++k) {
                for (unsigned int j = 0; j < n[k]; ++j) {
                    a1[k] *= a[k];
                }
            }
            Vector scale;
            Vector numerator;
            switch (mode) {
                case Mode::ONE_SIDED:
                    scale = (a - 1.0) / (a1 - 1.0);
                    numerator = Vc::sin(t) - a * Vc::sin(t - s) -
                                a1 * (Vc::sin(t + (n + 1.0) * s) - a * Vc::sin(t + n * s));
                    break;
                case Mode::TWO_SIDED:
                    scale = (a - 1.0) / (a1 + a1 - a - 1.0);
                    numerator = Vc::sin(t) * (1.0 - aa - (a1 + a1) * (Vc::cos((n + 1.0) * s) - a * Vc::cos(n * s)));
                    break;
            }
            Vector denominator = 1.0 + aa - (a + a) * Vc::cos(s);
            *outputIterator = scale * numerator / denominator;
#else
            Sample t = TAU * phaseIterator;
            Sample s = *modulationRatioIterator * t;
            Sample a = *intensityIterator;
            Sample n = *overtonesIterator;
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
            *outputIterator = scale * numerator / denominator;
#endif
            ++phaseIterator;
            ++modulationRatioIterator;
            ++intensityIterator;
            ++overtonesIterator;
            ++outputIterator;
        }
    }
}
