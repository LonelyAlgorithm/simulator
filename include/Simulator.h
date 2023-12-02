#ifndef Simulator_h
#define Simulator_h

#include "ISimulator.h"
#include "IUnit.h"
#include "SimpleUnit.h"
#include "IDisplay.h"

class Simulator : public ISimulator
{

public:
    Simulator(std::shared_ptr<IDisplay> &display_interface);

    void add_unit(const std::shared_ptr<IUnit> &unit) override;

    void remove_unit(const std::string &id) override;

    void compute() const override;

private:
    std::vector<std::shared_ptr<IUnit>> units_;

    std::shared_ptr<IDisplay> display_interface_;
};

#endif /* Simulator_h */