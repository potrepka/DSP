#pragma once

#include <atomic>

namespace dsp {

class Lockable {

public:
    void lock();
    void unlock();

private:
    std::atomic_flag flag = ATOMIC_FLAG_INIT;
};

} // namespace dsp
