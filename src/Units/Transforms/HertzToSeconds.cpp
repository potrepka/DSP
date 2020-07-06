#include "HertzToSeconds.h"

dsp::HertzToSeconds::HertzToSeconds() {
    getInputSignal()->setType(Type::HERTZ);
    getOutputSignal()->setType(Type::SECONDS);
}
