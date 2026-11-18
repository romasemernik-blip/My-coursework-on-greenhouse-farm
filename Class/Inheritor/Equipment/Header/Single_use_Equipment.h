#pragma once
#include "Class/Base/Header/Equipment.h"

class Single_use_equipment : public Equipment {
protected:
    double resource_left_;
    double resource_consumption_;

public:
    Single_use_equipment();
    Single_use_equipment(const std::string& name,
                         double energy_requirement,
                         double resource_left,
                         double resource_consumption);
    Single_use_equipment(const Single_use_equipment& other) = default;
    Single_use_equipment(Single_use_equipment&& other) noexcept = default;
    Single_use_equipment& operator=(const Single_use_equipment& other) = default;
    Single_use_equipment& operator=(Single_use_equipment&& other) noexcept = default;
    ~Single_use_equipment() override = default;

    void turn_on() override;
    void turn_off() override;

    void use(double amount);

    double get_resource_left()        const noexcept;
    double get_resource_consumption() const noexcept;

    void set_resource_left(double value);
    void set_resource_consumption(double value);
};