#include "Processor.h"

dsp::Processor::Processor(Type inType, Type outType, Space space)
        : Unit()
        , Consumer(inType, space)
        , Generator(outType, space) {}

#if DSP_USE_VC
void dsp::Processor::transform(std::function<Vector(Vector)> transform) {
#else
void dsp::Processor::transform(std::function<Sample(Sample)> transform) {
#endif
    for (unsigned int i = 0; i < getNumChannels(); ++i) {
        Array &inputBuffer = getInputSignal()->getChannel(i)->getBuffer();
        Array &outputBuffer = getOutputSignal()->getChannel(i)->getBuffer();
        Iterator inputIterator = inputBuffer.begin();
        Iterator outputIterator = outputBuffer.begin();
        while (outputIterator != outputBuffer.end()) {
            *outputIterator = transform(*inputIterator);
            ++inputIterator;
            ++outputIterator;
        }
    }
}

#if DSP_USE_VC
void dsp::Processor::transform(std::shared_ptr<InputParameter> parameter,
                               std::function<Vector(Vector, Vector)> transform) {
#else
void dsp::Processor::transform(std::shared_ptr<InputParameter> parameter,
                               std::function<Sample(Sample, Sample)> transform) {
#endif
    for (unsigned int i = 0; i < getNumChannels(); ++i) {
        Array &inputBuffer = getInputSignal()->getChannel(i)->getBuffer();
        Array &parameterBuffer = parameter->getChannel(i)->getBuffer();
        Array &outputBuffer = getOutputSignal()->getChannel(i)->getBuffer();
        Iterator inputIterator = inputBuffer.begin();
        Iterator parameterIterator = parameterBuffer.begin();
        Iterator outputIterator = outputBuffer.begin();
        while (outputIterator != outputBuffer.end()) {
            *outputIterator = transform(*inputIterator, *parameterIterator);
            ++inputIterator;
            ++parameterIterator;
            ++outputIterator;
        }
    }
}
