#include "Lockable.h"

void dsp::Lockable::lock() {
    while (flag.test_and_set(std::memory_order_acquire))
        ;
}

void dsp::Lockable::unlock() {
    flag.clear(std::memory_order_release);
}
