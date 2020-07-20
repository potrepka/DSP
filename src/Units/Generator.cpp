#include "Generator.h"

dsp::Generator::Generator(Type type, Space space)
        : outputSignal(pushOutput(type, space)) {}

std::shared_ptr<dsp::OutputParameter> dsp::Generator::getOutputSignal() const {
    return outputSignal;
}

void dsp::Generator::setOutputSignal(std::shared_ptr<OutputParameter> output) {
    replaceOutput(outputSignal, outputSignal = output);
}

#if DSP_USE_VC
void dsp::Generator::transformOutput(std::shared_ptr<InputParameter> parameter,
                                     std::function<Vector(Vector, Vector)> transform) {
#else
void dsp::Generator::transformOutput(std::shared_ptr<InputParameter> parameter,
                                     std::function<Sample(Sample, Sample)> transform) {
#endif
    for (unsigned int i = 0; i < getNumChannels(); ++i) {
        Array &parameterBuffer = parameter->getChannel(i)->getBuffer();
        Array &outputBuffer = getOutputSignal()->getChannel(i)->getBuffer();
        Iterator parameterIterator = parameterBuffer.begin();
        Iterator outputIterator = outputBuffer.begin();
        while (outputIterator != outputBuffer.end()) {
            *outputIterator = transform(*outputIterator, *parameterIterator);
            ++parameterIterator;
            ++outputIterator;
        }
    }
}
