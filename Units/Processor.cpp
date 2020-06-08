#include "Processor.h"

dsp::Processor::Processor(Connection::Type inType, Connection::Type outType) : Consumer(inType), Generator(outType) {}

void dsp::Processor::process() {
    Unit::process();
}
