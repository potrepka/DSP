#include "Crossover.h"

dsp::Crossover::Crossover()
        : input(std::make_shared<Identity>(Type::RATIO))
        , frequency(std::make_shared<Identity>(Type::HERTZ))
        , lp1(std::make_shared<Biquad>())
        , lp2(std::make_shared<Biquad>())
        , hp1(std::make_shared<Biquad>())
        , hp2(std::make_shared<Biquad>()) {
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
    Biquad::Mode::HIGH_PASS >> hp1->getMode();
    Biquad::Mode::HIGH_PASS >> hp2->getMode();
}

std::shared_ptr<dsp::Input> dsp::Crossover::getInput() const {
    return input->getInput();
}

std::shared_ptr<dsp::Input> dsp::Crossover::getFrequency() const {
    return frequency->getInput();
}

std::shared_ptr<dsp::Output> dsp::Crossover::getLow() const {
    return lp2->getOutput();
}

std::shared_ptr<dsp::Output> dsp::Crossover::getHigh() const {
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
