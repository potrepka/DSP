#include "SampleRate.h"

dsp::SampleRate::SampleRate() : Generator(Connection::Type::INTEGER) {}

void dsp::SampleRate::setSampleRate(unsigned int sampleRate) {
    Generator::setSampleRate(sampleRate);
    getOutputSignal()->setValue(sampleRate);
}
