#pragma once

#include "Constants.h"
#include "Lockable.h"

namespace dsp {

class Runnable : public Lockable {

public:
    Runnable();

    DSP_FLOAT getOneOverSampleRate();
    DSP_FLOAT getOneOverBufferSize();

    unsigned int getSampleRate();
    void setSampleRate(unsigned int sampleRate);

    unsigned int getBufferSize();
    void setBufferSize(unsigned int bufferSize);

protected:
    virtual void setSampleRateNoLock(unsigned int sampleRate);
    virtual void setBufferSizeNoLock(unsigned int bufferSize);

private:
    DSP_FLOAT oneOverSampleRate;
    DSP_FLOAT oneOverBufferSize;
    unsigned int sampleRate;
    unsigned int bufferSize;
};

} // namespace dsp
