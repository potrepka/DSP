#include "Crossover.h"

dsp::Crossover::Crossover()
        : input(std::make_shared<PassThrough>(Type::RATIO))
        , frequency(std::make_shared<PassThrough>(Type::HERTZ))
        , lowOne(std::make_shared<Biquad>())
        , lowTwo(std::make_shared<Biquad>())
        , highOne(std::make_shared<Biquad>())
        , highTwo(std::make_shared<Biquad>()) {
    highOne->setMode(Biquad::Mode::HIGH_PASS);
    highTwo->setMode(Biquad::Mode::HIGH_PASS);
    getChildren().push_back(input);
    getChildren().push_back(frequency);
    getChildren().push_back(lowOne);
    getChildren().push_back(lowTwo);
    getChildren().push_back(highOne);
    getChildren().push_back(highTwo);
    input->getOutput() >> lowOne->getInput();
    lowOne->getOutput() >> lowTwo->getInput();
    input->getOutput() >> highOne->getInput();
    highOne->getOutput() >> highTwo->getInput();
    frequency->getOutput() >> lowOne->getFrequency();
    frequency->getOutput() >> lowTwo->getFrequency();
    frequency->getOutput() >> highOne->getFrequency();
    frequency->getOutput() >> highTwo->getFrequency();
}

std::shared_ptr<dsp::Input> dsp::Crossover::getInput() {
    return input->getInput();
}

std::shared_ptr<dsp::Input> dsp::Crossover::getFrequency() {
    return frequency->getInput();
}

std::shared_ptr<dsp::Output> dsp::Crossover::getLow() {
    return lowTwo->getOutput();
}

std::shared_ptr<dsp::Output> dsp::Crossover::getHigh() {
    return highTwo->getOutput();
}

void dsp::Crossover::setNumInputChannelsNoLock(int numChannels) {
    Node::setNumInputChannelsNoLock(numChannels);
    for (const auto &child : getChildren()) {
        child->setNumInputChannels(numChannels);
    }
}

void dsp::Crossover::setNumOutputChannelsNoLock(int numChannels) {
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
