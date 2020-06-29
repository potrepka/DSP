#include "PassThrough.h"

dsp::PassThrough::PassThrough(Connection::Type type, Connection::Space space) : Identity(type, type, space) {}
