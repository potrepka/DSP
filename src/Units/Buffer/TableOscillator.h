#pragma once

#include "Buffer.h"
#include "Generator.h"

namespace dsp {

class TableOscillator : public Generator {

public:
    TableOscillator();
    unsigned int getNumTables();
    std::shared_ptr<Buffer> getTable(unsigned int index);
    void setTable(unsigned int index, std::shared_ptr<Buffer> table);
    void pushTable(std::shared_ptr<Buffer> table);
    void insertTable(unsigned int index, std::shared_ptr<Buffer> table);
    void removeTable(unsigned int index);
    std::shared_ptr<InputParameter> getPhase();
    std::shared_ptr<InputParameter> getPosition();

protected:
    void process() override;

private:
    static const unsigned int PHASE;
    static const unsigned int POSITION;
    std::vector<std::shared_ptr<Buffer>> tables;
};

} // namespace dsp
