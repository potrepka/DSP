#pragma once

#include "Biquad.h"
#include "Consumer.h"
#include "PassThrough.h"

namespace dsp {

class Crossover : public Consumer {

public:
    Crossover();
    ~Crossover();

    std::shared_ptr<InputParameter> getFrequency();
    std::shared_ptr<OutputParameter> getLow();
    std::shared_ptr<OutputParameter> getHigh();

protected:
    void connect() override;
    void disconnect() override;

private:
    const std::shared_ptr<PassThrough> input;
    const std::shared_ptr<PassThrough> frequency;
    const std::shared_ptr<Biquad> low1;
    const std::shared_ptr<Biquad> low2;
    const std::shared_ptr<Biquad> high1;
    const std::shared_ptr<Biquad> high2;
};

} // namespace dsp
