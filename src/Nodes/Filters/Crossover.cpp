#include "Crossover.h"

dsp::Crossover::Crossover()
        : input(std::make_shared<PassThrough>(Type::RATIO))
        , frequency(std::make_shared<PassThrough>(Type::HERTZ))
        , lp1(std::make_shared<Biquad>())
        , lp2(std::make_shared<Biquad>())
        , hp1(std::make_shared<Biquad>())
        , hp2(std::make_shared<Biquad>()) {
    hp1->setMode(Biquad::Mode::HIGH_PASS);
    hp2->setMode(Biquad::Mode::HIGH_PASS);
    getInputs().push_back(input->getInput());
    getInputs().push_back(frequency->getInput());
    getOutputs().push_back(lp2->getOutput());
    getOutputs().push_back(hp2->getOutput());
    getChildren().push_back(input);
    getChildren().push_back(frequency);
    getChildren().push_back(lp1);
    getChildren().push_back(lp2);
    getChildren().push_back(hp1);
    getChildren().push_back(hp2);
    input->getOutput() >> lp1->getInput();
    input->getOutput() >> hp1->getInput();
    frequency->getOutput() >> lp1->getFrequency();
    frequency->getOutput() >> lp2->getFrequency();
    frequency->getOutput() >> hp1->getFrequency();
    frequency->getOutput() >> hp2->getFrequency();
    lp1->getOutput() >> lp2->getInput();
    hp1->getOutput() >> hp2->getInput();
}

std::shared_ptr<dsp::Input> dsp::Crossover::getInput() {
    return input->getInput();
}

std::shared_ptr<dsp::Input> dsp::Crossover::getFrequency() {
    return frequency->getInput();
}

std::shared_ptr<dsp::Output> dsp::Crossover::getLow() {
    return lp2->getOutput();
}

std::shared_ptr<dsp::Output> dsp::Crossover::getHigh() {
    return hp2->getOutput();
}

void dsp::Crossover::setNumInputChannelsNoLock(size_t numChannels) {
    Node::setNumInputChannelsNoLock(numChannels);
    for (const auto &child : getChildren()) {
        child->setNumInputChannels(numChannels);
    }
}

void dsp::Crossover::setNumOutputChannelsNoLock(size_t numChannels) {
    Node::setNumOutputChannelsNoLock(numChannels);
    for (const auto &child : getChildren()) {
        child->setNumOutputChannels(numChannels);
    }
}

void dsp::Crossover::processNoLock() {
    for (const auto &child : getChildren()) {
        child->process();
    }
}