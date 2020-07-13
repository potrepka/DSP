#include "PassThrough.h"

dsp::PassThrough::PassThrough(Type type, Space space)
        : Processor(type, type, space) {}

void dsp::PassThrough::process() {
    Unit::process();
    for (unsigned int i = 0; i < getNumChannels(); i++) {
        getOutputSignal()->getChannel(i)->getBuffer() = getInputSignal()->getChannel(i)->getBuffer();
    }
}
