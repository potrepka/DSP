#include "Transformer.h"

dsp::Transformer::Transformer(Type type, Space space)
        : input(std::make_shared<Input>(type, space))
        , output(std::make_shared<Output>(type, space)) {
    getInputs().push_back(input);
    getOutputs().push_back(output);
}

dsp::Transformer::Transformer(Type inType, Type outType, Space space)
        : input(std::make_shared<Input>(inType, space))
        , output(std::make_shared<Output>(outType, space)) {
    getInputs().push_back(input);
    getOutputs().push_back(output);
}

dsp::Transformer::Transformer(Type inType, Type outType, Space inSpace, Space outSpace)
        : input(std::make_shared<Input>(inType, inSpace))
        , output(std::make_shared<Output>(outType, outSpace)) {
    getInputs().push_back(input);
    getOutputs().push_back(output);
}

std::shared_ptr<dsp::Input> dsp::Transformer::getInput() const {
    return input;
}

std::shared_ptr<dsp::Output> dsp::Transformer::getOutput() const {
    return output;
}
