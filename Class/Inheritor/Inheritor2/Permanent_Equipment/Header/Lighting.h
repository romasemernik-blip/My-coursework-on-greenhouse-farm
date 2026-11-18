#pragma once
#include "Class/Inheritor/Equipment/Header/Permanent_Equipment.h"

class Lighting : public Permanent_equipment {
protected:
    double brightness_;
    double spectrum_nm_;

public:
    Lighting();
    Lighting(const std::string& name,
             double energy_requirement,
             double brightness,
             double spectrum_nm);
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