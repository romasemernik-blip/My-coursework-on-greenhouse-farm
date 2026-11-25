#include "Class/Base/Header/Human.h"
#include "Class/Exception/Header/All_exception.h"

Human::Human(std::string_view first_name, std::string_view last_name, std::string_view middle_name, std::size_t birth_date, std::string_view gender)
    : first_name_(first_name), last_name_(last_name), middle_name_(middle_name), birth_date_(birth_date), gender_(gender) 
{
    if (first_name.empty()) throw Invalid_argument_exception("first_name", "must not be empty");
    if (last_name.empty()) throw Invalid_argument_exception("last_name", "must not be empty");
}

std::string Human::get_full_name() const {
    std::string result = last_name_ + " " + first_name_;
    if (!middle_name_.empty()) result += " " + middle_name_;
    return result;
}

const std::string& Human::get_first_name()  const noexcept { return first_name_; }
const std::string& Human::get_last_name()   const noexcept { return last_name_; }
const std::string& Human::get_middle_name() const noexcept { return middle_name_; }
std::size_t        Human::get_birth_date()  const noexcept { return birth_date_; }
const std::string& Human::get_gender()      const noexcept { return gender_; }

void Human::set_first_name(std::string_view value) {
    if (value.empty()) throw Invalid_argument_exception("first_name", "must not be empty");
    first_name_ = value;
}

void Human::set_last_name(std::string_view value) {
    if (value.empty()) throw Invalid_argument_exception("last_name", "must not be empty");
    last_name_ = value;
}

void Human::set_middle_name(std::string_view value) { middle_name_ = value; }
void Human::set_birth_date(std::size_t value) { birth_date_ = value; }
void Human::set_gender(std::string_view value) { gender_ = value; }