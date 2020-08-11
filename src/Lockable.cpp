#include "Lockable.h"

dsp::Lockable::~Lockable() {}

void dsp::Lockable::lock() {
    mtx.lock();
}

void dsp::Lockable::unlock() {
    mtx.unlock();
}
