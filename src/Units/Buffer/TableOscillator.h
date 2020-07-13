#pragma once

#include "Buffer.h"
#include "Generator.h"

namespace dsp {

class TableOscillator : public Generator {

public:
    TableOscillator(Type type);

    unsigned int getNumTables() const;
    std::shared_ptr<Buffer> getTable(unsigned int index) const;
    void setTable(unsigned int index, std::shared_ptr<Buffer> table);
    void pushTable(std::shared_ptr<Buffer> table);
    void insertTable(unsigned int index, std::shared_ptr<Buffer> table);
    void removeTable(unsigned int index);
    std::shared_ptr<InputParameter> getPhase() const;
    std::shared_ptr<InputParameter> getPosition() const;

protected:
    void process() override;

private:
    const unsigned int PHASE;
    const unsigned int POSITION;

    std::vector<std::shared_ptr<Buffer>> tables;
};

} // namespace dsp
