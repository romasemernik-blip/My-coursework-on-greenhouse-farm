#include "Class/Inheritor/Inheritor2/Permanent_Equipment/Header/Lighting.h"
#include "Class/Exception/Header/All_exception.h"

Lighting::Lighting()
    : brightness_(0.0)
    , spectrum_nm_(0.0)
{}

Lighting::Lighting(const std::string& name,
                   double energy_requirement,
                   double brightness,
                   double spectrum_nm)
    : Permanent_equipment(name, energy_requirement)
    , brightness_(brightness)
    , spectrum_nm_(spectrum_nm)
{
    if (brightness < 0.0 || brightness > 100.0)
        throw Invalid_argument_exception("brightness", "must be in [0, 100]");
}

std::string Lighting::get_type() const { return "Lighting"; }

double Lighting::get_brightness()  const noexcept { return brightness_; }
double Lighting::get_spectrum_nm() const noexcept { return spectrum_nm_; }

void Lighting::set_brightness(double value) {
    if (value < 0.0 || value > 100.0)
        throw Invalid_argument_exception("brightness", "must be in [0, 100]");
    brightness_ = value;
}

void Lighting::set_spectrum(double nm) {
    if (nm < 380.0 || nm > 780.0)
        throw Invalid_argument_exception("spectrum_nm", "must be in [380, 780]");
    spectrum_nm_ = nm;
}