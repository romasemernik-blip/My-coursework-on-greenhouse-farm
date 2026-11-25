#include "Class/Base/Header/Weather.h"

Weather::Weather(const std::string& name, double moisture_effect, double ph_effect, double growth_modifier)
    : name_(name), moisture_effect_(moisture_effect), ph_effect_(ph_effect), growth_modifier_(growth_modifier) {}

const std::string& Weather::get_name() const noexcept { return name_; }
double Weather::get_moisture_effect() const noexcept { return moisture_effect_; }
double Weather::get_ph_effect() const noexcept { return ph_effect_; }
double Weather::get_growth_modifier() const noexcept { return growth_modifier_; }