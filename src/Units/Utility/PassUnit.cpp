#include "PassUnit.h"

dsp::PassUnit::PassUnit(Connection::Type type) : Identity(type, type) {}
