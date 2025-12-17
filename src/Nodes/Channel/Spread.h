#pragma once

#include "../../Core/Transformer.h"

namespace dsp {

class Spread : public Transformer {

public:
    struct Mode {
        static constexpr int MIN = 0;
        static constexpr int MAX = 1;
        static constexpr int UNIPOLAR = 0;
        static constexpr int BIPOLAR = 1;
    };

    Spread(Type type = Type::RATIO, Space space = Space::TIME);

    std::shared_ptr<Input> getSpread() const;
    std::shared_ptr<Input> getMode() const;

protected:
    void processNoLock() override;

private:
    const std::shared_ptr<Input> spread;
    const std::shared_ptr<Input> mode;
};

} // namespace dsp
