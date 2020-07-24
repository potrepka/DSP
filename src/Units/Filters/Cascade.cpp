#include "Cascade.h"

dsp::Cascade::Cascade()
        : Processor(Type::BIPOLAR, Type::BIPOLAR)
        , input(std::make_shared<PassThrough>(Type::BIPOLAR))
        , frequency(std::make_shared<PassThrough>(Type::HERTZ))
        , resonance(std::make_shared<PassThrough>(Type::RATIO))
        , gain(std::make_shared<PassThrough>(Type::LINEAR))
        , output(std::make_shared<PassThrough>(Type::BIPOLAR))
        , mode(Mode::LOW_PASS)
        , halfOrder(0) {
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
    pushUnit(output);

    connect();
}

dsp::Cascade::~Cascade() {
    disconnect();
}

dsp::Cascade::Mode dsp::Cascade::getMode() const {
    return mode;
}

void dsp::Cascade::setMode(Mode mode) {
    lock();
    this->mode = mode;
    for (const auto &biquad : biquads) {
        setMode(biquad);
    }
    unlock();
}

unsigned int dsp::Cascade::getOrder() const {
    return halfOrder << 1;
}

void dsp::Cascade::setOrder(unsigned int order) {
    lock();
    disconnect();
    halfOrder = order >> 1;
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
    Sample increment = PI_OVER_TWO / halfOrder;
    Sample firstAngle = 0.5 * increment;
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
    connect();
    unlock();
}

std::shared_ptr<dsp::InputParameter> dsp::Cascade::getFrequency() const {
    return frequency->getInputSignal();
}

std::shared_ptr<dsp::InputParameter> dsp::Cascade::getResonance() const {
    return resonance->getInputSignal();
}

std::shared_ptr<dsp::InputParameter> dsp::Cascade::getGain() const {
    return gain->getInputSignal();
}

void dsp::Cascade::connect() {
    if (halfOrder == 0) {
        input->getOutputSignal() >> output->getInputSignal();
    } else {
        input->getOutputSignal() >> biquads[0]->getInputSignal();
        biquads[halfOrder - 1]->getOutputSignal() >> output->getInputSignal();
    }
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

void dsp::Cascade::disconnect() {
    if (halfOrder == 0) {
        input->getOutputSignal() != output->getInputSignal();
    } else {
        input->getOutputSignal() != biquads[0]->getInputSignal();
        biquads[halfOrder - 1]->getOutputSignal() != output->getInputSignal();
    }
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

void dsp::Cascade::setMode(std::shared_ptr<Biquad> biquad) {
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
