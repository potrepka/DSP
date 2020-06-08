#pragma once

#include "Filter.h"

namespace dsp {

class ChannelMix : public Filter {

public:
    ChannelMix();
    void setBufferSize(unsigned int bufferSize) override;
    std::shared_ptr<InputParameter> getMix();

protected:
    void process() override;

private:
    static const std::size_t MIX;
    std::vector<DSP_FLOAT> buffer;
};

} // namespace dsp
