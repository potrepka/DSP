#include "HertzToSeconds.h"

dsp::HertzToSeconds::HertzToSeconds() {
    getInputSignal()->setType(Connection::Type::HERTZ);
    getOutputSignal()->setType(Connection::Type::SECONDS);
}
