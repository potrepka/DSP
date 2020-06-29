#include "Processor.h"

dsp::Processor::Processor(Connection::Type inType, Connection::Type outType, Connection::Space space)
        : Consumer(inType, space), Generator(outType, space) {}
