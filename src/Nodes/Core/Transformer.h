#pragma once

#include "Consumer.h"
#include "Producer.h"

namespace dsp {

class Transformer : public Consumer, public Producer {

public:
    Transformer(Type inType, Type outType, Space space = Space::TIME);
    Transformer(Type inType, Type outType, Space inSpace, Space outSpace);

protected:
    void transform(std::function<Sample(Sample)> transform);
    void transform(std::shared_ptr<Input> parameter, std::function<Sample(Sample, Sample)> transform);
};

} // namespace dsp
