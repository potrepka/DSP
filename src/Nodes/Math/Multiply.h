#pragma once

#include "../Core/Transformer.h"

namespace dsp {

class Multiply : public Transformer {

public:
    Multiply(Type type = Type::RATIO, Space space = Space::TIME);

    size_t getNumFactors() const;
    void setNumFactors(size_t numFactors);

    std::vector<std::shared_ptr<Input>> &getFactors();

protected:
    void processNoLock() override;

private:
    std::vector<std::shared_ptr<Input>> factors;
};

} // namespace dsp
