#include "PassThroughChain.h"

dsp::PassThroughChain::PassThroughChain()
        : Processor(Type::BIPOLAR, Type::BIPOLAR)
        , input(std::make_shared<PassThrough>(Type::BIPOLAR))
        , output(std::make_shared<PassThrough>(Type::BIPOLAR))
        , order(0) {
    setInputSignal(input->getInputSignal());
    setOutputSignal(output->getOutputSignal());
    pushUnit(input);
    pushUnit(output);
    connect();
}

unsigned int dsp::PassThroughChain::getOrder() const {
    return order;
}

void dsp::PassThroughChain::setOrder(unsigned int order) {
    lock();
    disconnect();
    removeUnitNoLock(output);
    for (const auto &pass : passes) {
        removeUnitNoLock(pass);
    }
    passes.clear();
    passes.reserve(order);
    for (unsigned int i = 0; i < order; ++i) {
        std::shared_ptr<PassThrough> pass = std::make_shared<PassThrough>(Type::BIPOLAR);
        passes.push_back(pass);
        pushUnitNoLock(pass);
    }
    pushUnitNoLock(output);
    this->order = order;
    connect();
    unlock();
}

void dsp::PassThroughChain::connect() {
    if (order == 0) {
        input->getOutputSignal() >> output->getInputSignal();
    } else {
        input->getOutputSignal() >> passes[0]->getInputSignal();
        passes[order - 1]->getOutputSignal() >> output->getInputSignal();
    }
    for (unsigned int i = 0; i + 1 < order; ++i) {
        passes[i]->getOutputSignal() >> passes[i + 1]->getInputSignal();
    }
}

void dsp::PassThroughChain::disconnect() {
    if (order == 0) {
        input->getOutputSignal() != output->getInputSignal();
    } else {
        input->getOutputSignal() != passes[0]->getInputSignal();
        passes[order - 1]->getOutputSignal() != output->getInputSignal();
    }
    for (unsigned int i = 0; i + 1 < order; ++i) {
        passes[i]->getOutputSignal() != passes[i + 1]->getInputSignal();
    }
}
