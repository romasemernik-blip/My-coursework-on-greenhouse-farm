#include "Class/Inheritor/Inheritor2/Single_use_Equipment/Header/Watering_Equipment.h"

Watering_equipment::Watering_equipment() = default;

Watering_equipment::Watering_equipment(const std::string& name,
                                       double energy_requirement,
                                       double water_left,
                                       double water_consumption)
    : Single_use_equipment(name, energy_requirement,
                           water_left, water_consumption)
{}

std::string Watering_equipment::get_type() const { return "Watering_equipment"; }

void Watering_equipment::water(double liters) {
    use(liters);
}