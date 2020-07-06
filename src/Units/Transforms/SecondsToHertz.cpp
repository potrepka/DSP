#include "SecondsToHertz.h"

dsp::SecondsToHertz::SecondsToHertz() {
    getInputSignal()->setType(Type::SECONDS);
    getOutputSignal()->setType(Type::HERTZ);
}
