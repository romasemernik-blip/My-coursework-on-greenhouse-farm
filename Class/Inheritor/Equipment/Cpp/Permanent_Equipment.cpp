#include "Class/Inheritor/Equipment/Header/Permanent_Equipment.h"
#include "Class/Exception/Header/All_exception.h"

Permanent_equipment::Permanent_equipment() = default;

Permanent_equipment::Permanent_equipment(const std::string& name,
                                         double energy_requirement)
    : Equipment(name, energy_requirement)
{}

void Permanent_equipment::turn_on() {
    if (is_broken_)
        throw Equipment_failure_exception(name_, wear_level_);
    is_on_ = true;
}

void Permanent_equipment::turn_off() {
    is_on_ = false;
}