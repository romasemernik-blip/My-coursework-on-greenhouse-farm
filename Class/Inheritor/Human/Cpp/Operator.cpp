#include "Class/Inheritor/Human/Header/Operator.h"
#include "Class/Exception/Header/All_exception.h"
#include "Class/Config/Header/Config.h"

Operator::Operator()
    : shift_("Day")
    , access_level_(1)
    , is_on_duty_(false)
{}

Operator::Operator(const std::string& first_name,
                   const std::string& last_name,
                   const std::string& middle_name,
                   std::size_t birth_date,
                   const std::string& gender,
                   const std::string& shift,
                   int access_level)
    : Human(first_name, last_name, middle_name, birth_date, gender)
    , shift_(shift)
    , access_level_(access_level)
    , is_on_duty_(false)
{
    if (shift.empty())
        throw Invalid_argument_exception("shift", "must not be empty");
    if (access_level < 0 || access_level > 5)
        throw Invalid_argument_exception("access_level", "must be in [0, 5]");
}

Operator::Operator(Operator&& other) noexcept = default;
Operator& Operator::operator=(Operator&& other) noexcept = default;

std::string Operator::get_role() const { return "Operator"; }

const std::string& Operator::get_shift() const noexcept { return shift_; }
int Operator::get_access_level() const noexcept { return access_level_; }
bool Operator::get_is_on_duty() const noexcept { return is_on_duty_; }

void Operator::set_shift(const std::string& value) {
    if (value.empty())
        throw Invalid_argument_exception("shift", "must not be empty");
    shift_ = value;
}

void Operator::set_access_level(int value) {
    if (value < 0 || value > 5)
        throw Invalid_argument_exception("access_level", "must be in [0, 5]");
    access_level_ = value;
}

void Operator::set_is_on_duty(bool value) {
    is_on_duty_ = value;
}

void Operator::toggle_duty() {
    is_on_duty_ = !is_on_duty_;
}