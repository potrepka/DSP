#pragma once

#include <mutex>

namespace dsp {

class Lockable {

public:
    virtual void lock();
    virtual void unlock();

private:
    std::mutex mtx;

};

}
