#include "Transformer.h"

dsp::Transformer::Transformer(Type type, Domain domain)
    : input(std::make_shared<Input>(type, domain)),
      output(std::make_shared<Output>(type, domain)) {
  getInputs().push_back(input);
  getOutputs().push_back(output);
}

dsp::Transformer::Transformer(Type inputType, Type outputType, Domain domain)
    : input(std::make_shared<Input>(inputType, domain)),
      output(std::make_shared<Output>(outputType, domain)) {
  getInputs().push_back(input);
  getOutputs().push_back(output);
}

dsp::Transformer::Transformer(Type inputType, Type outputType, Domain inDomain,
                              Domain outDomain)
    : input(std::make_shared<Input>(inputType, inDomain)),
      output(std::make_shared<Output>(outputType, outDomain)) {
  getInputs().push_back(input);
  getOutputs().push_back(output);
}

std::shared_ptr<dsp::Input> dsp::Transformer::getInput() const { return input; }

std::shared_ptr<dsp::Output> dsp::Transformer::getOutput() const {
  return output;
}
