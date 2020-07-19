#pragma once

#include "Constants.h"
#include "Lockable.h"

namespace dsp {

class Runnable : public Lockable {

public:
    Runnable();

    Sample getOneOverSampleRate() const;
    Sample getOneOverBufferSize() const;

    unsigned int getSampleRate() const;
    void setSampleRate(unsigned int sampleRate);

    unsigned int getBufferSize() const;
    void setBufferSize(unsigned int bufferSize);

protected:
    virtual void setSampleRateNoLock(unsigned int sampleRate);
    virtual void setBufferSizeNoLock(unsigned int bufferSize);

private:
    Sample oneOverSampleRate;
    Sample oneOverBufferSize;
    unsigned int sampleRate;
    unsigned int bufferSize;
};

} // namespace dsp
