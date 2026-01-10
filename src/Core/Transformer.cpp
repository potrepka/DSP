#include "Transformer.h"

dsp::Transformer::Transformer(Type type, Space space)
    : input(std::make_shared<Input>(type, space)),
      output(std::make_shared<Output>(type, space)) {
  getInputs().push_back(input);
  getOutputs().push_back(output);
}

dsp::Transformer::Transformer(Type inputType, Type outputType, Space space)
    : input(std::make_shared<Input>(inputType, space)),
      output(std::make_shared<Output>(outputType, space)) {
  getInputs().push_back(input);
  getOutputs().push_back(output);
}

dsp::Transformer::Transformer(Type inputType, Type outputType, Space inSpace,
                              Space outSpace)
    : input(std::make_shared<Input>(inputType, inSpace)),
      output(std::make_shared<Output>(outputType, outSpace)) {
  getInputs().push_back(input);
  getOutputs().push_back(output);
}

std::shared_ptr<dsp::Input> dsp::Transformer::getInput() const { return input; }

std::shared_ptr<dsp::Output> dsp::Transformer::getOutput() const {
  return output;
}
