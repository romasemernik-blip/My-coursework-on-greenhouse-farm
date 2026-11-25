#pragma once
#include <string>

class Weather {
protected:
    std::string name_;
    double moisture_effect_; // Изменение влажности в час
    double ph_effect_;       // Изменение pH в час
    double growth_modifier_; // Множитель роста (1.0 = норма)

public:
    Weather(const std::string& name, double moisture_effect, double ph_effect, double growth_modifier);
    virtual ~Weather() = default;

    const std::string& get_name() const noexcept;
    double get_moisture_effect() const noexcept;
    double get_ph_effect() const noexcept;
    double get_growth_modifier() const noexcept;
};