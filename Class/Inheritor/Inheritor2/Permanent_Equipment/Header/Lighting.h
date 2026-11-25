#pragma once
#include "Class/Inheritor/Equipment/Header/Permanent_Equipment.h"
#include <string_view>

class Lighting : public Permanent_equipment {
private:
    double brightness_ = 0.0;
    double spectrum_nm_ = 0.0;

public:
    Lighting() = default;
    Lighting(std::string_view name, double energy_requirement, double brightness, double spectrum_nm);
    Lighting(const Lighting& other) = default;
    Lighting(Lighting&& other) noexcept = default;
    Lighting& operator=(const Lighting& other) = default;
    Lighting& operator=(Lighting&& other) noexcept = default;
    ~Lighting() override = default;

    std::string get_type() const override;

    double get_brightness()  const noexcept;
    double get_spectrum_nm() const noexcept;

    void set_brightness(double value);
    void set_spectrum(double nm);
};