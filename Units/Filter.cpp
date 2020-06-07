#include "Filter.h"

dsp::Filter::Filter(Connection::Type type) : Consumer(type), Generator(type) {}

std::size_t dsp::Filter::getNumChannels() {
    std::size_t numInputChannels = getInputSignal()->getNumChannels();
    std::size_t numOutputChannels = getOutputSignal()->getNumChannels();
    return numInputChannels < numOutputChannels ? numInputChannels : numOutputChannels;
}

void dsp::Filter::process() {
    Consumer::process();
    Generator::process();
}
