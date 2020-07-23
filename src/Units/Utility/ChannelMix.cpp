#include "ChannelMix.h"

dsp::ChannelMix::ChannelMix(Type type, Space space)
        : Consumer(type, space)
        , mixAmount(pushInput(Type::UNIPOLAR, space))
        , mid(pushOutput(type, space))
        , side(pushOutput(type, space)) {
    assert(type != Type::BINARY);
    assert(type != Type::INTEGER);
}

std::shared_ptr<dsp::InputParameter> dsp::ChannelMix::getMixAmount() const {
    return mixAmount;
}

std::shared_ptr<dsp::OutputParameter> dsp::ChannelMix::getMid() const {
    return mid;
}

std::shared_ptr<dsp::OutputParameter> dsp::ChannelMix::getSide() const {
    return side;
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
            Array &midBuffer = getMid()->getChannel(i)->getBuffer();
            Array &sideBuffer = getSide()->getChannel(i)->getBuffer();
            Iterator bufferIterator = buffer.begin();
            Iterator inputIterator = inputBuffer.begin();
            Iterator mixAmountIterator = mixAmountBuffer.begin();
            Iterator midIterator = midBuffer.begin();
            Iterator sideIterator = sideBuffer.begin();
            while (inputIterator != inputBuffer.end()) {
#if DSP_USE_VC
                Vector bufferValue = *bufferIterator;
#else
                Sample bufferValue = *bufferIterator;
#endif
                *midIterator = *inputIterator + *mixAmountIterator * (bufferValue - *inputIterator);
                *sideIterator = *inputIterator + *mixAmountIterator * -bufferValue;
                ++bufferIterator;
                ++inputIterator;
                ++mixAmountIterator;
                ++midIterator;
                ++sideIterator;
            }
        }
    }
}
