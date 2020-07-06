#include "Processor.h"

dsp::Processor::Processor(Connection::Type inType, Connection::Type outType, Connection::Space space)
        : Consumer(inType, space), Generator(outType, space) {}

void dsp::Processor::transform(std::function<DSP_FLOAT(DSP_FLOAT)> transform) {
    for (unsigned int i = 0; i < getNumChannels(); i++) {
        std::transform(getInputSignal()->getChannel(i)->getBuffer().begin(),
                       getInputSignal()->getChannel(i)->getBuffer().end(),
                       getOutputSignal()->getChannel(i)->getBuffer().begin(),
                       transform);
    }
}

void dsp::Processor::transform(std::shared_ptr<InputParameter> input,
                               std::function<DSP_FLOAT(DSP_FLOAT, DSP_FLOAT)> transform) {
    for (unsigned int i = 0; i < getNumChannels(); i++) {
        std::transform(getInputSignal()->getChannel(i)->getBuffer().begin(),
                       getInputSignal()->getChannel(i)->getBuffer().end(),
                       input->getChannel(i)->getBuffer().begin(),
                       getOutputSignal()->getChannel(i)->getBuffer().begin(),
                       transform);
    }
}
