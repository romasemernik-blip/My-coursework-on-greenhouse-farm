#pragma once
#include "Class/Base/Header/Equipment.h"
#include <string_view>

class Single_use_equipment : public Equipment {
private:
    double resource_left_ = 0.0;
    double resource_consumption_ = 0.0;

public:
    Single_use_equipment() = default;
    Single_use_equipment(std::string_view name, double energy_requirement, double resource_left, double resource_consumption);
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