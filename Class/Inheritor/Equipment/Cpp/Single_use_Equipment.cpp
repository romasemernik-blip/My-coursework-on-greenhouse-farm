#include "Class/Inheritor/Equipment/Header/Single_use_Equipment.h"
#include "Class/Exception/Header/All_exception.h"

Single_use_equipment::Single_use_equipment()
    : resource_left_(0.0)
    , resource_consumption_(0.0)
{}

Single_use_equipment::Single_use_equipment(const std::string& name,
                                           double energy_requirement,
                                           double resource_left,
                                           double resource_consumption)
    : Equipment(name, energy_requirement)
    , resource_left_(resource_left)
    , resource_consumption_(resource_consumption)
{
    if (resource_left < 0.0)
        throw Invalid_argument_exception("resource_left", "must be non-negative");
    if (resource_consumption < 0.0)
        throw Invalid_argument_exception("resource_consumption", "must be non-negative");
}

void Single_use_equipment::turn_on() {
    if (is_broken_)
        throw Equipment_failure_exception(name_, wear_level_);
    if (resource_left_ <= 0.0)
        throw Invalid_argument_exception("resource_left", "no resource left");
    is_on_ = true;
}

void Single_use_equipment::turn_off() {
    is_on_ = false;
}

void Single_use_equipment::use(double amount) {
    if (amount < 0.0)
        throw Invalid_argument_exception("amount", "must be non-negative");
    if (amount > resource_left_)
        throw Invalid_argument_exception("amount", "not enough resource");
    resource_left_ -= amount;
    is_on_ = false;
}

double Single_use_equipment::get_resource_left()        const noexcept { return resource_left_; }
double Single_use_equipment::get_resource_consumption() const noexcept { return resource_consumption_; }

void Single_use_equipment::set_resource_left(double value) {
    if (value < 0.0)
        throw Invalid_argument_exception("resource_left", "must be non-negative");
    resource_left_ = value;
}

void Single_use_equipment::set_resource_consumption(double value) {
    if (value < 0.0)
        throw Invalid_argument_exception("resource_consumption", "must be non-negative");
    resource_consumption_ = value;
}