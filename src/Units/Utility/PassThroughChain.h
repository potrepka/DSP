#pragma once

#include "PassThrough.h"

namespace dsp {

class PassThroughChain : public Processor {

public:
    PassThroughChain();

    unsigned int getOrder() const;
    void setOrder(unsigned int order);

protected:
    void connect() override;
    void disconnect() override;

private:
    const std::shared_ptr<PassThrough> input;
    const std::shared_ptr<PassThrough> output;

    unsigned int order;
    std::vector<std::shared_ptr<PassThrough>> passes;
};

} // namespace dsp
