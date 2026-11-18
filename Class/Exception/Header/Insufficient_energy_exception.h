#pragma once
#include "Class/Exception/Header/Greenhouse_exception.h"
#include <sstream>
#include <iomanip>

class Insufficient_energy_exception : public Greenhouse_exception {
private:
    double required_energy_;
    double available_energy_;
public:
    Insufficient_energy_exception(double required_energy,
                                   double available_energy)
        : Greenhouse_exception(make_message(required_energy, available_energy))
        , required_energy_(required_energy)
        , available_energy_(available_energy)
    {}

    double required_energy()  const noexcept { return required_energy_; }
    double available_energy() const noexcept { return available_energy_; }

private:
    static std::string make_message(double req, double avail) {
        std::ostringstream os;
        os << std::fixed << std::setprecision(2)
           << "Insufficient energy: required " << req
           << " kWh, available " << avail << " kWh";
        return os.str();
    }
};