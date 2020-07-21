#include "Butterworth.h"

dsp::Butterworth::Butterworth()
        : Processor(Type::BIPOLAR, Type::BIPOLAR)
        , input(std::make_shared<PassThrough>(Type::BIPOLAR))
        , frequency(std::make_shared<PassThrough>(Type::HERTZ))
        , resonance(std::make_shared<PassThrough>(Type::RATIO))
        , gain(std::make_shared<PassThrough>(Type::LINEAR))
        , onePole(std::make_shared<OnePole>(Type::BIPOLAR))
        , output(std::make_shared<PassThrough>(Type::BIPOLAR))
        , mode(Mode::LOW_PASS)
        , order(1) {
    1.0 >> resonance->getInputSignal();

    setInputSignal(input->getInputSignal());
    pushInput(frequency->getInputSignal());
    pushInput(resonance->getInputSignal());
    pushInput(gain->getInputSignal());

    setOutputSignal(output->getOutputSignal());

    pushUnit(input);
    pushUnit(frequency);
    pushUnit(resonance);
    pushUnit(gain);
    pushUnit(onePole);
    pushUnit(output);

    connect();
}

dsp::Butterworth::Mode dsp::Butterworth::getMode() const {
    return mode;
}

void dsp::Butterworth::setMode(Mode mode) {
    lock();
    this->mode = mode;
    switch (mode) {
        case Mode::LOW_PASS: onePole->setMode(OnePole::Mode::LOW_PASS); break;
        case Mode::HIGH_PASS: onePole->setMode(OnePole::Mode::HIGH_PASS); break;
        default: break;
    }
    for (const auto &biquad : biquads) {
        setMode(biquad);
    }
    if (order % 2 && mode != Mode::LOW_PASS && mode != Mode::HIGH_PASS) {
        setActiveNoLock(false);
    } else {
        setActiveNoLock(true);
    }
    unlock();
}

unsigned int dsp::Butterworth::getOrder() const {
    return order;
}

void dsp::Butterworth::setOrder(unsigned int order) {
    lock();
    disconnect();
    unsigned int halfOrder = order >> 1;
    removeUnitNoLock(output);
    for (const auto &multiply : multiplies) {
        removeUnitNoLock(multiply);
    }
    for (const auto &biquad : biquads) {
        removeUnitNoLock(biquad);
    }
    multiplies.clear();
    biquads.clear();
    multiplies.reserve(halfOrder);
    biquads.reserve(halfOrder);
    Sample increment = PI / order;
    Sample firstAngle = increment;
    if (order % 2 == 0) {
        firstAngle *= 0.5;
    }
    for (unsigned int i = 0; i < halfOrder; ++i) {
        std::shared_ptr<Multiply> multiply = std::make_shared<Multiply>(Type::RATIO);
        1.0 / (2.0 * cos(firstAngle + i * increment)) >> multiply->pushInputRatio();
        multiplies.push_back(multiply);
        pushUnitNoLock(multiply);

        std::shared_ptr<Biquad> biquad = std::make_shared<Biquad>();
        setMode(biquad);
        biquads.push_back(biquad);
        pushUnitNoLock(biquad);
    }
    pushUnitNoLock(output);
    if (order % 2 && mode != Mode::LOW_PASS && mode != Mode::HIGH_PASS) {
        setActiveNoLock(false);
    } else {
        setActiveNoLock(true);
    }
    this->order = order;
    connect();
    unlock();
}

std::shared_ptr<dsp::InputParameter> dsp::Butterworth::getFrequency() const {
    return frequency->getInputSignal();
}

std::shared_ptr<dsp::InputParameter> dsp::Butterworth::getResonance() const {
    return resonance->getInputSignal();
}

std::shared_ptr<dsp::InputParameter> dsp::Butterworth::getGain() const {
    return gain->getInputSignal();
}

void dsp::Butterworth::connect() {
    unsigned int halfOrder = order >> 1;
    if (order == 0) {
        input->getOutputSignal() >> output->getInputSignal();
    } else if (order == 1) {
        input->getOutputSignal() >> onePole->getInputSignal();
        onePole->getOutputSignal() >> output->getInputSignal();
    } else if (order % 2 == 0) {
        input->getOutputSignal() >> biquads[0]->getInputSignal();
        biquads[halfOrder - 1]->getOutputSignal() >> output->getInputSignal();
    } else {
        input->getOutputSignal() >> onePole->getInputSignal();
        onePole->getOutputSignal() >> biquads[0]->getInputSignal();
        biquads[halfOrder - 1]->getOutputSignal() >> output->getInputSignal();
    }
    frequency->getOutputSignal() >> onePole->getFrequency();
    for (unsigned int i = 0; i < halfOrder; ++i) {
        std::shared_ptr<Multiply> multiply = multiplies[i];
        std::shared_ptr<Biquad> biquad = biquads[i];
        frequency->getOutputSignal() >> biquad->getFrequency();
        resonance->getOutputSignal() >> multiply->getInputSignal();
        multiply->getOutputSignal() >> biquad->getResonance();
        gain->getOutputSignal() >> biquad->getGain();
        if (i + 1 < halfOrder) {
            biquads[i]->getOutputSignal() >> biquads[i + 1]->getInputSignal();
        }
    }
}

void dsp::Butterworth::disconnect() {
    unsigned int halfOrder = order >> 1;
    if (order == 0) {
        input->getOutputSignal() != output->getInputSignal();
    } else if (order == 1) {
        input->getOutputSignal() != onePole->getInputSignal();
        onePole->getOutputSignal() != output->getInputSignal();
    } else if (order % 2 == 0) {
        input->getOutputSignal() != biquads[0]->getInputSignal();
        biquads[halfOrder - 1]->getOutputSignal() != output->getInputSignal();
    } else if (mode == Mode::LOW_PASS || mode == Mode::HIGH_PASS) {
        input->getOutputSignal() != onePole->getInputSignal();
        onePole->getOutputSignal() != biquads[0]->getInputSignal();
        biquads[halfOrder - 1]->getOutputSignal() != output->getInputSignal();
    }
    frequency->getOutputSignal() != onePole->getFrequency();
    for (unsigned int i = 0; i < halfOrder; ++i) {
        std::shared_ptr<Multiply> multiply = multiplies[i];
        std::shared_ptr<Biquad> biquad = biquads[i];
        frequency->getOutputSignal() != biquad->getFrequency();
        resonance->getOutputSignal() != multiply->getInputSignal();
        multiply->getOutputSignal() != biquad->getResonance();
        gain->getOutputSignal() != biquad->getGain();
        if (i + 1 < halfOrder) {
            biquads[i]->getOutputSignal() != biquads[i + 1]->getInputSignal();
        }
    }
}

void dsp::Butterworth::setMode(std::shared_ptr<Biquad> biquad) {
    switch (mode) {
        case Mode::LOW_PASS: biquad->setMode(Biquad::Mode::LOW_PASS); break;
        case Mode::HIGH_PASS: biquad->setMode(Biquad::Mode::HIGH_PASS); break;
        case Mode::BAND_PASS: biquad->setMode(Biquad::Mode::BAND_PASS); break;
        case Mode::BAND_STOP: biquad->setMode(Biquad::Mode::BAND_STOP); break;
        case Mode::LOW_SHELF: biquad->setMode(Biquad::Mode::LOW_SHELF); break;
        case Mode::HIGH_SHELF: biquad->setMode(Biquad::Mode::HIGH_SHELF); break;
        case Mode::PEAK: biquad->setMode(Biquad::Mode::PEAK); break;
        case Mode::ALL_PASS: biquad->setMode(Biquad::Mode::ALL_PASS); break;
    }
}
