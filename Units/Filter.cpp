#include "Filter.h"

dsp::Filter::Filter(Connection::Type type) : Consumer(type), Generator(type) {}

void dsp::Filter::process() {
    Unit::process();
}
