#pragma once

#include "../Core/Transformer.h"

namespace dsp {

class Clipper : public Transformer {

public:
    struct Mode {
        static const int MIN = 0;
        static const int MAX = 2;
        static const int CLIP = 0;
        static const int WRAP = 1;
        static const int MIRROR = 2;
    };

    Clipper(Type type = Type::RATIO, Space space = Space::TIME);

    std::shared_ptr<Input> getMinimum() const;
    std::shared_ptr<Input> getMaximum() const;
    std::shared_ptr<Input> getMode() const;

protected:
    void processNoLock() override;

private:
    std::shared_ptr<Input> minimum;
    std::shared_ptr<Input> maximum;
    std::shared_ptr<Input> mode;
};

} // namespace dsp
