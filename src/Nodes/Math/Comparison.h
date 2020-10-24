#pragma once

#include "../Core/Transformer.h"

namespace dsp {

class Comparison : public Transformer {

public:
    struct Mode {
        static const int MIN = -1;
        static const int MAX = 1;
        static const int EQUAL_TO = 0;
        static const int GREATER_THAN = 1;
        static const int LESS_THAN = -1;
    };

    Comparison(Type type = Type::RATIO, Space space = Space::TIME);

    std::shared_ptr<Input> getThreshold() const;
    std::shared_ptr<Input> getMode() const;

protected:
    void processNoLock() override;

private:
    const std::shared_ptr<Input> threshold;
    const std::shared_ptr<Input> mode;
};

} // namespace dsp
