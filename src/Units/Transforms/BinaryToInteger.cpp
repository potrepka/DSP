#include "BinaryToInteger.h"

dsp::BinaryToInteger::BinaryToInteger(Connection::Space space)
        : Identity(Connection::Type::BINARY, Connection::Type::INTEGER, space) {}
