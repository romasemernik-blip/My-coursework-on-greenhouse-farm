#include "Class/Inheritor/Inheritor2/Single_use_Equipment/Header/Fertilizer_Equipment.h"

Fertilizer_equipment::Fertilizer_equipment(std::string_view name, double energy_requirement, double fertilizer_left, double fertilizer_consumption)
    : Single_use_equipment(name, energy_requirement, fertilizer_left, fertilizer_consumption) {}

std::string Fertilizer_equipment::get_type() const { return "Fertilizer_equipment"; }
void Fertilizer_equipment::fertilize(double kilograms) { use(kilograms); }