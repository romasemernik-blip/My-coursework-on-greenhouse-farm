#pragma once
#include <string>
#include <string_view>

class Weather {
private:
    std::string name_;
    double moisture_effect_ = 0.0;
    double ph_effect_ = 0.0;
    double growth_modifier_ = 1.0;

public:
    Weather(std::string_view name, double moisture_effect, double ph_effect, double growth_modifier);
    virtual ~Weather() = default;

    const std::string& get_name() const noexcept;
    double get_moisture_effect() const noexcept;
    double get_ph_effect() const noexcept;
    double get_growth_modifier() const noexcept;
};