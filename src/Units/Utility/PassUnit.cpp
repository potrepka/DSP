#include "PassUnit.h"

dsp::PassUnit::PassUnit(Connection::Type type, Connection::Space space) : Identity(type, type, space) {}
