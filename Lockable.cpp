#include "Lockable.h"

void dsp::Lockable::lock() {
    mtx.lock();
}

void dsp::Lockable::unlock() {
    mtx.unlock();
}
