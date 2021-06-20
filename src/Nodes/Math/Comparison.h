#pragma once

#include "../Core/Transformer.h"

namespace dsp {

class Comparison : public Transformer {

public:
    Comparison(Type type = Type::RATIO, Space space = Space::TIME);

    std::shared_ptr<Input> getThreshold() const;

protected:
    void processNoLock() override;

private:
    const std::shared_ptr<Input> threshold;
};

} // namespace dsp
