#include "OneOver.h"

dsp::OneOver::OneOver(Type type, Space space)
        : Transformer(type,
                      type == Type::HERTZ ? Type::SECONDS : type == Type::SECONDS ? Type::HERTZ : type,
                      space) {}

void dsp::OneOver::processNoLock() {
    transform([](Sample x) { return 1.0 / x; });
}
