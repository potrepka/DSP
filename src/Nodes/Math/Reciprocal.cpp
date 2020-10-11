#include "Reciprocal.h"

dsp::Reciprocal::Reciprocal(Type type, Space space)
        : Transformer(type,
                      type == Type::HERTZ ? Type::SECONDS : type == Type::SECONDS ? Type::HERTZ : type,
                      space) {}

void dsp::Reciprocal::processNoLock() {
    transform([](Sample x) { return 1.0 / x; });
}
