#include "BinaryToLinear.h"

dsp::BinaryToLinear::BinaryToLinear(Connection::Space space)
        : Identity(Connection::Type::BINARY, Connection::Type::LINEAR, space) {}
