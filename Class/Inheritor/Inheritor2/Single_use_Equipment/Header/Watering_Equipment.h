#pragma once
#include "Class/Inheritor/Equipment/Header/Single_use_Equipment.h"

class Watering_equipment : public Single_use_equipment {
public:
    Watering_equipment();
    Watering_equipment(const std::string& name,
                       double energy_requirement,
                       double water_left,
                       double water_consumption);
    Watering_equipment(const Watering_equipment& other) = default;
    Watering_equipment(Watering_equipment&& other) noexcept = default;
    Watering_equipment& operator=(const Watering_equipment& other) = default;
    Watering_equipment& operator=(Watering_equipment&& other) noexcept = default;
    ~Watering_equipment() override = default;

    std::string get_type() const override;
    void water(double liters);
};