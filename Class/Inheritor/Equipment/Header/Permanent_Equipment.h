#pragma once
#include "Class/Base/Header/Equipment.h"

class Permanent_equipment : public Equipment {
public:
    Permanent_equipment();
    Permanent_equipment(const std::string& name, double energy_requirement);
    Permanent_equipment(const Permanent_equipment& other) = default;
    Permanent_equipment(Permanent_equipment&& other) noexcept = default;
    Permanent_equipment& operator=(const Permanent_equipment& other) = default;
    Permanent_equipment& operator=(Permanent_equipment&& other) noexcept = default;
    ~Permanent_equipment() override = default;

    void turn_on() override;
    void turn_off() override;
};