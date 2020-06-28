#include "SecondsToHertz.h"

dsp::SecondsToHertz::SecondsToHertz() {
    getInputSignal()->setType(Connection::Type::SECONDS);
    getOutputSignal()->setType(Connection::Type::HERTZ);
}
