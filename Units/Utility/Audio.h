#pragma once

#include "Unit.h"

namespace dsp {

class Audio : public Unit {

public:
    Audio();
    std::size_t getNumChannels() override;
    std::shared_ptr<OutputParameter> getAudioInput();
    std::shared_ptr<InputParameter> getAudioOutput();

protected:
    void process() override;

};

}
