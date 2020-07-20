#include "ChannelMix.h"

dsp::ChannelMix::ChannelMix(Type type, Space space)
        : Processor(type, type, space)
        , mixAmount(pushInput(Type::UNIPOLAR, space)) {
    assert(type != Type::BINARY);
    assert(type != Type::INTEGER);
}

dsp::ChannelMix::Mode dsp::ChannelMix::getMode() const {
    return mode;
}

void dsp::ChannelMix::setMode(Mode mode) {
    lock();
    this->mode = mode;
    unlock();
}

std::shared_ptr<dsp::InputParameter> dsp::ChannelMix::getMixAmount() const {
    return mixAmount;
}

void dsp::ChannelMix::setBufferSizeNoLock(unsigned int bufferSize) {
    Unit::setBufferSizeNoLock(bufferSize);
    buffer.resize(bufferSize);
}

void dsp::ChannelMix::process() {
    Unit::process();
    if (getNumChannels() > 0) {
        std::vector<Iterator> iterators(getNumChannels());
        for (unsigned int i = 0; i < getNumChannels(); ++i) {
            iterators[i] = getInputSignal()->getChannel(i)->getBuffer().begin();
        }
        for (Iterator it = buffer.begin(); it != buffer.end(); ++it) {
#if DSP_USE_VC
            Vector sum = Vector::Zero();
#else
            Sample sum = 0.0;
#endif
            for (Iterator &iterator : iterators) {
                sum += *iterator;
                ++iterator;
            }
            *it = sum / getNumChannels();
        }
        for (unsigned int i = 0; i < getNumChannels(); ++i) {
            Array &inputBuffer = getInputSignal()->getChannel(i)->getBuffer();
            Array &mixAmountBuffer = getMixAmount()->getChannel(i)->getBuffer();
            Array &outputBuffer = getOutputSignal()->getChannel(i)->getBuffer();
            Iterator mixIterator = buffer.begin();
            Iterator inputIterator = inputBuffer.begin();
            Iterator mixAmountIterator = mixAmountBuffer.begin();
            Iterator outputIterator = outputBuffer.begin();
            while (outputIterator != outputBuffer.end()) {
#if DSP_USE_VC
                Vector mix = *mixIterator;
                Vector wet;
#else
                Sample mix = *mixIterator;
                Sample wet;
#endif
                switch (mode) {
                    case Mode::MID: wet = mix - *inputIterator; break;
                    case Mode::SIDE: wet = -mix; break;
                }
                *outputIterator = *inputIterator + *mixAmountIterator * wet;
                ++mixIterator;
                ++inputIterator;
                ++mixAmountIterator;
                ++outputIterator;
            }
        }
    }
}
