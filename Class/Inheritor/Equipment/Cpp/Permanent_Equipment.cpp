#include "Class/Inheritor/Equipment/Header/Permanent_Equipment.h"
#include "Class/Exception/Header/All_exception.h"

Permanent_equipment::Permanent_equipment(std::string_view name, double energy_requirement)
    : Equipment(name, energy_requirement) {}

void Permanent_equipment::turn_on() {
    if (get_is_broken())
        throw Equipment_failure_exception(get_name(), get_wear_level());
    set_is_on(true);
}

void Permanent_equipment::turn_off() {
    set_is_on(false);
}