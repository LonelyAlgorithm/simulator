#include "Simulator.h"

Simulator::Simulator(std::shared_ptr<IDisplay> &display_interface)
{
    display_interface_ = display_interface;
}

void Simulator::add_unit(const std::shared_ptr<IUnit> &unit)
{

    if (unit)
    {
        display_interface_->addGeometry(RobotBody(unit->get_id(), unit->get_position(), unit->get_orientation(), 0.05, {0.5, 0.5, 0.5}), true);
        units_.push_back(unit);
    }
    else
    {
        throw std::runtime_error("Unsupported geometry type.");
    }
}

void Simulator::remove_unit(const std::string &id)
{
    for (auto it = units_.begin(); it != units_.end();)
    {
        const std::shared_ptr<SimpleUnit> simpleUnitPtr = std::dynamic_pointer_cast<SimpleUnit>(*it);
        if (simpleUnitPtr)
        {
            if (simpleUnitPtr->get_id() == id)
            {
                it = units_.erase(it);
                display_interface_->removeGeometryByName(id, true);
            }
            else
            {
                ++it;
            }
        }
        else
        {
            ++it;
        }
    }
}

void Simulator::compute() const
{
    for (auto unit : units_)
    {
        unit->move(std::chrono::duration_cast<std::chrono::duration<float>>(std::chrono::high_resolution_clock::now().time_since_epoch()).count());
    }
}
