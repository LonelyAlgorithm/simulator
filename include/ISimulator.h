#ifndef ISimulator_h
#define ISimulator_h

#include "IUnit.h"
#include "string"
#include <chrono>
#include "vector"

class ISimulator
{

public:
    virtual void add_unit(const std::shared_ptr<IUnit> &unit) = 0;

    virtual void remove_unit(const std::string &id) = 0;

    virtual void compute() const = 0;
};

#endif /* ISimulator_h */