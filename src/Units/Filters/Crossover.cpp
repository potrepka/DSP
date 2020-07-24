#include "Crossover.h"

dsp::Crossover::Crossover()
        : Consumer(Type::BIPOLAR)
        , input(std::make_shared<PassThrough>(Type::BIPOLAR))
        , frequency(std::make_shared<PassThrough>(Type::HERTZ))
        , low1(std::make_shared<Biquad>())
        , low2(std::make_shared<Biquad>())
        , high1(std::make_shared<Biquad>())
        , high2(std::make_shared<Biquad>()) {
    setInputSignal(input->getInputSignal());
    pushInput(frequency->getInputSignal());

    pushUnit(input);
    pushUnit(frequency);
    pushUnit(low1);
    pushUnit(low2);
    pushUnit(high1);
    pushUnit(high2);

    connect();
}

dsp::Crossover::~Crossover() {
    disconnect();
}

std::shared_ptr<dsp::InputParameter> dsp::Crossover::getFrequency() {
    return frequency->getInputSignal();
}

std::shared_ptr<dsp::OutputParameter> dsp::Crossover::getLow() {
    return low2->getOutputSignal();
}

std::shared_ptr<dsp::OutputParameter> dsp::Crossover::getHigh() {
    return high2->getOutputSignal();
}

void dsp::Crossover::connect() {
    input->getOutputSignal() >> low1->getInputSignal();
    low1->getOutputSignal() >> low2->getInputSignal();
    input->getOutputSignal() >> high1->getInputSignal();
    high1->getOutputSignal() >> high2->getInputSignal();
    frequency->getOutputSignal() >> low1->getFrequency();
    frequency->getOutputSignal() >> low2->getFrequency();
    frequency->getOutputSignal() >> high1->getFrequency();
    frequency->getOutputSignal() >> high2->getFrequency();
}

void dsp::Crossover::disconnect() {
    input->getOutputSignal() != low1->getInputSignal();
    low1->getOutputSignal() != low2->getInputSignal();
    input->getOutputSignal() != high1->getInputSignal();
    high1->getOutputSignal() != high2->getInputSignal();
    frequency->getOutputSignal() != low1->getFrequency();
    frequency->getOutputSignal() != low2->getFrequency();
    frequency->getOutputSignal() != high1->getFrequency();
    frequency->getOutputSignal() != high2->getFrequency();
}
