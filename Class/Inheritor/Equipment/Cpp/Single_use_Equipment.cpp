#include "Class/Inheritor/Equipment/Header/Single_use_Equipment.h"
#include "Class/Exception/Header/All_exception.h"

Single_use_equipment::Single_use_equipment(std::string_view name, double energy_requirement, double resource_left, double resource_consumption)
    : Equipment(name, energy_requirement), resource_left_(resource_left), resource_consumption_(resource_consumption) 
{
    if (resource_left < 0.0) throw Invalid_argument_exception("resource_left", "must be non-negative");
    if (resource_consumption < 0.0) throw Invalid_argument_exception("resource_consumption", "must be non-negative");
}

void Single_use_equipment::turn_on() {
    if (get_is_broken()) throw Equipment_failure_exception(get_name(), get_wear_level());
    if (resource_left_ <= 0.0) throw Invalid_argument_exception("resource_left", "no resource left");
    set_is_on(true);
}

void Single_use_equipment::turn_off() { 
    set_is_on(false); 
}

void Single_use_equipment::use(double amount) {
    if (amount < 0.0) throw Invalid_argument_exception("amount", "must be non-negative");
    if (amount > resource_left_) throw Invalid_argument_exception("amount", "not enough resource");
    resource_left_ -= amount;
    set_is_on(false);
}

double Single_use_equipment::get_resource_left()        const noexcept { return resource_left_; }
double Single_use_equipment::get_resource_consumption() const noexcept { return resource_consumption_; }

void Single_use_equipment::set_resource_left(double value) {
    if (value < 0.0) throw Invalid_argument_exception("resource_left", "must be non-negative");
    resource_left_ = value;
}

void Single_use_equipment::set_resource_consumption(double value) {
    if (value < 0.0) throw Invalid_argument_exception("resource_consumption", "must be non-negative");
    resource_consumption_ = value;
}