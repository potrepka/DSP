#pragma once

#include "../../Core/Transformer.h"

namespace dsp {

class Clipper : public Transformer {

public:
    struct Mode {
        static constexpr int MIN = 0;
        static constexpr int MAX = 2;
        static constexpr int CLIP = 0;
        static constexpr int WRAP = 1;
        static constexpr int FOLD = 2;
    };

    Clipper(Type type = Type::RATIO, Space space = Space::TIME);

    std::shared_ptr<Input> getMin() const;
    std::shared_ptr<Input> getMax() const;
    std::shared_ptr<Input> getMode() const;

protected:
    void processNoLock() override;

private:
    std::shared_ptr<Input> min;
    std::shared_ptr<Input> max;
    std::shared_ptr<Input> mode;
};

} // namespace dsp
