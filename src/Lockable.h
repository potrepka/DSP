#pragma once

#include <mutex>

namespace dsp {

class Lockable {

public:
    virtual ~Lockable() = 0;

    virtual void lock();
    virtual void unlock();

private:
    std::mutex mtx;
};

} // namespace dsp
