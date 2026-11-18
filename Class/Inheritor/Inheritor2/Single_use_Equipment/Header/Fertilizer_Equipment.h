#pragma once
#include "Class/Inheritor/Equipment/Header/Single_use_Equipment.h"

class Fertilizer_equipment : public Single_use_equipment {
public:
    Fertilizer_equipment();
    Fertilizer_equipment(const std::string& name,
                         double energy_requirement,
                         double fertilizer_left,
                         double fertilizer_consumption);
    Fertilizer_equipment(const Fertilizer_equipment& other) = default;
    Fertilizer_equipment(Fertilizer_equipment&& other) noexcept = default;
    Fertilizer_equipment& operator=(const Fertilizer_equipment& other) = default;
    Fertilizer_equipment& operator=(Fertilizer_equipment&& other) noexcept = default;
    ~Fertilizer_equipment() override = default;

    std::string get_type() const override;
    void fertilize(double kilograms);
};