#pragma once

#include "Processor.h"

namespace dsp {

class Identity : public Processor {

public:
    Identity(Connection::Type inType, Connection::Type outType, Connection::Space space = Connection::Space::TIME);

protected:
    void process() override;
};

class BinaryToInteger : public Identity {

public:
    BinaryToInteger(Connection::Space space = Connection::Space::TIME);
};

class BinaryToLinear : public Identity {

public:
    BinaryToLinear(Connection::Space space = Connection::Space::TIME);
};

class BinaryToRatio : public Identity {

public:
    BinaryToRatio(Connection::Space space = Connection::Space::TIME);
};

class IntegerToLinear : public Identity {

public:
    IntegerToLinear(Connection::Space space = Connection::Space::TIME);
};

class IntegerToRatio : public Identity {

public:
    IntegerToRatio(Connection::Space space = Connection::Space::TIME);
};

} // namespace dsp
