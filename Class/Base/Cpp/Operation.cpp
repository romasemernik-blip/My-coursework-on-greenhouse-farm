#include "Class/Base/Header/Operation.h"
#include "Class/Exception/Header/All_exception.h"

Operation::Operation(std::string_view name, double energy_cost, std::unique_ptr<Human> worker, std::unique_ptr<Equipment> equipment)
    : name_(name), energy_cost_(energy_cost), worker_(std::move(worker)), equipment_(std::move(equipment)) 
{
    if (name.empty()) throw Invalid_argument_exception("name", "must not be empty");
    if (energy_cost < 0.0) throw Invalid_argument_exception("energy_cost", "must be non-negative");
}

const std::string& Operation::get_name()        const noexcept { return name_; }
double             Operation::get_energy_cost() const noexcept { return energy_cost_; }
Human*             Operation::get_worker()      noexcept { return worker_.get(); }
const Human*       Operation::get_worker()      const noexcept { return worker_.get(); }
Equipment*         Operation::get_equipment()   noexcept { return equipment_.get(); }
const Equipment*   Operation::get_equipment()   const noexcept { return equipment_.get(); }

void Operation::set_name(std::string_view value) {
    if (value.empty()) throw Invalid_argument_exception("name", "must not be empty");
    name_ = value;
}

void Operation::set_energy_cost(double value) {
    if (value < 0.0) throw Invalid_argument_exception("energy_cost", "must be non-negative");
    energy_cost_ = value;
}

void Operation::set_worker(std::unique_ptr<Human> worker) { worker_ = std::move(worker); }
void Operation::set_equipment(std::unique_ptr<Equipment> equipment) { equipment_ = std::move(equipment); }