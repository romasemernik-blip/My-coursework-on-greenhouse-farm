#include "Class/Base/Header/Equipment.h"
#include "Class/Exception/Header/All_exception.h"

Equipment::Equipment()
    : is_broken_(false)
    , is_on_(false)
    , energy_requirement_(0.0)
    , wear_level_(0.0)
{}

Equipment::Equipment(const std::string& name, double energy_requirement)
    : name_(name)
    , is_broken_(false)
    , is_on_(false)
    , energy_requirement_(energy_requirement)
    , wear_level_(0.0)
{
    if (name.empty())
        throw Invalid_argument_exception("name", "must not be empty");
    if (energy_requirement < 0.0)
        throw Invalid_argument_exception("energy_requirement", "must be non-negative");
}

Equipment::Equipment(const Equipment& other) = default;
Equipment::Equipment(Equipment&& other) noexcept = default;
Equipment& Equipment::operator=(const Equipment& other) = default;
Equipment& Equipment::operator=(Equipment&& other) noexcept = default;

void Equipment::apply_wear(double amount) {
    if (amount < 0.0)
        throw Invalid_argument_exception("amount", "must be non-negative");
    wear_level_ += amount;
    if (wear_level_ >= 100.0) {
        wear_level_ = 100.0;
        is_broken_ = true;
        is_on_ = false;
    }
}

void Equipment::repair() {
    is_broken_ = false;
    wear_level_ = 0.0;
}

const std::string& Equipment::get_name()               const noexcept { return name_; }
bool               Equipment::get_is_broken()          const noexcept { return is_broken_; }
bool               Equipment::get_is_on()              const noexcept { return is_on_; }
double             Equipment::get_energy_requirement() const noexcept { return energy_requirement_; }
double             Equipment::get_wear_level()         const noexcept { return wear_level_; }

void Equipment::set_name(const std::string& value) {
    if (value.empty())
        throw Invalid_argument_exception("name", "must not be empty");
    name_ = value;
}

void Equipment::set_is_broken(bool value) {
    is_broken_ = value;
}

void Equipment::set_is_on(bool value) {
    is_on_ = value;
}

void Equipment::set_energy_requirement(double value) {
    if (value < 0.0)
        throw Invalid_argument_exception("energy_requirement", "must be non-negative");
    energy_requirement_ = value;
}

void Equipment::set_wear_level(double value) {
    if (value < 0.0 || value > 100.0)
        throw Invalid_argument_exception("wear_level", "must be in [0, 100]");
    wear_level_ = value;
}