#include "Identity.h"

dsp::Identity::Identity(Type type, Domain domain) : Transformer(type, domain) {}

dsp::Identity::Identity(Type inputType, Type outputType, Domain domain)
    : Transformer(inputType, outputType, domain) {}

dsp::Identity::Identity(Type inputType, Type outputType, Domain inputDomain,
                        Domain outputDomain)
    : Transformer(inputType, outputType, inputDomain, outputDomain) {}

void dsp::Identity::processNoLock() {
  getOutput()->getWrapper().copyFrom(getInput()->getWrapper());
}
