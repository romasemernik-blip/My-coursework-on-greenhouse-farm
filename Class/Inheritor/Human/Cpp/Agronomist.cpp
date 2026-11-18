#include "Class/Inheritor/Human/Header/Agronomist.h"
#include "Class/Exception/Header/All_exception.h"

Agronomist::Agronomist()
    : start_year_(0)
    , experience_years_(0)
{}

Agronomist::Agronomist(const std::string& first_name,
                       const std::string& last_name,
                       const std::string& middle_name,
                       std::size_t birth_date,
                       const std::string& gender,
                       int start_year,
                       int experience_years)
    : Human(first_name, last_name, middle_name, birth_date, gender)
    , start_year_(start_year)
    , experience_years_(experience_years)
{
    if (experience_years < 0)
        throw Invalid_argument_exception("experience_years", "must be non-negative");
}

Agronomist::Agronomist(Agronomist&& other) noexcept = default;
Agronomist& Agronomist::operator=(Agronomist&& other) noexcept = default;

std::string Agronomist::get_role() const { return "Agronomist"; }

int Agronomist::get_start_year()       const noexcept { return start_year_; }
int Agronomist::get_experience_years() const noexcept { return experience_years_; }
std::size_t Agronomist::get_bed_count() const noexcept { return assigned_beds_.size(); }

void Agronomist::set_start_year(int value) {
    start_year_ = value;
}

void Agronomist::set_experience_years(int value) {
    if (value < 0)
        throw Invalid_argument_exception("experience_years", "must be non-negative");
    experience_years_ = value;
}

void Agronomist::add_bed(std::unique_ptr<Bed> bed) {
    if (!bed)
        throw Invalid_argument_exception("bed", "must not be null");
    assigned_beds_.push_back(std::move(bed));
}

void Agronomist::clear_beds() {
    assigned_beds_.clear();
}