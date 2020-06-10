#include "Processor.h"

dsp::Processor::Processor(Connection::Type inType, Connection::Type outType) : Consumer(inType), Generator(outType) {}
