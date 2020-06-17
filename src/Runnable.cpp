#include "Runnable.h"

dsp::Runnable::Runnable() : oneOverSampleRate(0.0), sampleRate(0), bufferSize(0) {}

DSP_FLOAT dsp::Runnable::getOneOverSampleRate() {
    return oneOverSampleRate;
}

unsigned int dsp::Runnable::getSampleRate() {
    return sampleRate;
}

void dsp::Runnable::setSampleRate(unsigned int sampleRate) {
    lock();
    setSampleRateNoLock(sampleRate);
    unlock();
}

unsigned int dsp::Runnable::getBufferSize() {
    return bufferSize;
}

void dsp::Runnable::setBufferSize(unsigned int bufferSize) {
    lock();
    setBufferSizeNoLock(bufferSize);
    unlock();
}

void dsp::Runnable::setSampleRateNoLock(unsigned int sampleRate) {
    this->sampleRate = sampleRate;
    oneOverSampleRate = 1.0 / sampleRate;
}

void dsp::Runnable::setBufferSizeNoLock(unsigned int bufferSize) {
    this->bufferSize = bufferSize;
}
