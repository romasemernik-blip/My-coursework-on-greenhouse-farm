#include "Class/Inheritor/Inheritor2/Permanent_Equipment/Header/Sensor.h"
#include "Class/Exception/Header/All_exception.h"

Sensor::Sensor(std::string_view name, double energy_requirement, std::string_view units, double min_value, double max_value)
    : Permanent_equipment(name, energy_requirement), units_(units), min_value_(min_value), max_value_(max_value) 
{
    if (min_value >= max_value) 
        throw Invalid_argument_exception("min_value", "must be less than max_value");
}

double             Sensor::get_last_value() const noexcept { return last_value_; }
const std::string& Sensor::get_units()      const noexcept { return units_; }
double             Sensor::get_min_value()  const noexcept { return min_value_; }
double             Sensor::get_max_value()  const noexcept { return max_value_; }

void Sensor::set_last_value(double value) { last_value_ = value; }
void Sensor::set_units(std::string_view value) { units_ = value; }

void Sensor::set_min_value(double value) {
    if (value >= max_value_) 
        throw Invalid_argument_exception("min_value", "must be less than max_value");
    min_value_ = value;
}

void Sensor::set_max_value(double value) {
    if (value <= min_value_) 
        throw Invalid_argument_exception("max_value", "must be greater than min_value");
    max_value_ = value;
}