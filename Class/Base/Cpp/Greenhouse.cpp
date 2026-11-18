#include "Class/Base/Header/Greenhouse.h"
#include "Class/Exception/Header/All_exception.h"

Greenhouse::Greenhouse()
    : length_(0.0)
    , width_(0.0)
    , height_(0.0)
    , max_energy_(0.0)
    , current_energy_(0.0)
{}

Greenhouse::Greenhouse(const std::string& name,
                       double length,
                       double width,
                       double height,
                       double max_energy)
    : name_(name)
    , length_(length)
    , width_(width)
    , height_(height)
    , max_energy_(max_energy)
    , current_energy_(max_energy)
{
    if (name.empty())
        throw Invalid_argument_exception("name", "must not be empty");
    if (length <= 0.0 || width <= 0.0 || height <= 0.0)
        throw Invalid_argument_exception("dimensions", "must be positive");
    if (max_energy < 0.0)
        throw Invalid_argument_exception("max_energy", "must be non-negative");
}

Greenhouse::Greenhouse(Greenhouse&& other) noexcept = default;
Greenhouse& Greenhouse::operator=(Greenhouse&& other) noexcept = default;

double Greenhouse::get_total_area() const noexcept {
    return length_ * width_;
}

double Greenhouse::get_used_area() const noexcept {
    double total = 0.0;
    for (std::size_t i = 0; i < beds_.size(); ++i)
        total += beds_[i]->get_area();
    return total;
}

double Greenhouse::get_free_area() const noexcept {
    double free_area = get_total_area() - get_used_area();
    return free_area < 0.0 ? 0.0 : free_area;
}

void Greenhouse::add_bed(std::unique_ptr<Bed> bed) {
    if (!bed)
        throw Invalid_argument_exception("bed", "must not be null");
    double need = bed->get_area();
    double free_area = get_free_area();
    if (need > free_area)
        throw Greenhouse_full_exception(
            static_cast<std::size_t>(need),
            static_cast<std::size_t>(free_area));
    beds_.push_back(std::move(bed));
}

void Greenhouse::add_equipment(std::unique_ptr<Equipment> equipment) {
    if (!equipment)
        throw Invalid_argument_exception("equipment", "must not be null");
    equipment_.push_back(std::move(equipment));
}

std::unique_ptr<Bed> Greenhouse::remove_bed(std::size_t index) {
    if (index >= beds_.size())
        throw Bed_not_found_exception(index);
    auto result = std::move(beds_[index]);
    beds_.remove_at(index);
    return result;
}

std::unique_ptr<Equipment> Greenhouse::remove_equipment(std::size_t index) {
    if (index >= equipment_.size())
        throw Invalid_argument_exception("index", "equipment index out of range");
    auto result = std::move(equipment_[index]);
    equipment_.remove_at(index);
    return result;
}

void Greenhouse::clear_beds()     { beds_.clear(); }
void Greenhouse::clear_equipment() { equipment_.clear(); }

void Greenhouse::consume_energy(double amount) {
    if (amount < 0.0)
        throw Invalid_argument_exception("amount", "must be non-negative");
    if (amount > current_energy_)
        throw Insufficient_energy_exception(amount, current_energy_);
    current_energy_ -= amount;
}

void Greenhouse::restore_energy(double amount) {
    if (amount < 0.0)
        throw Invalid_argument_exception("amount", "must be non-negative");
    current_energy_ += amount;
    if (current_energy_ > max_energy_) current_energy_ = max_energy_;
}

const std::string& Greenhouse::get_name()           const noexcept { return name_; }
double             Greenhouse::get_length()         const noexcept { return length_; }
double             Greenhouse::get_width()          const noexcept { return width_; }
double             Greenhouse::get_height()         const noexcept { return height_; }
double             Greenhouse::get_max_energy()     const noexcept { return max_energy_; }
double             Greenhouse::get_current_energy() const noexcept { return current_energy_; }
std::size_t        Greenhouse::get_bed_count()      const noexcept { return beds_.size(); }
std::size_t        Greenhouse::get_equipment_count() const noexcept { return equipment_.size(); }

Bed* Greenhouse::get_bed(std::size_t index) {
    if (index >= beds_.size())
        throw Bed_not_found_exception(index);
    return beds_[index].get();
}

const Bed* Greenhouse::get_bed(std::size_t index) const {
    if (index >= beds_.size())
        throw Bed_not_found_exception(index);
    return beds_[index].get();
}

Equipment* Greenhouse::get_equipment(std::size_t index) {
    if (index >= equipment_.size())
        throw Invalid_argument_exception("index", "equipment index out of range");
    return equipment_[index].get();
}

const Equipment* Greenhouse::get_equipment(std::size_t index) const {
    if (index >= equipment_.size())
        throw Invalid_argument_exception("index", "equipment index out of range");
    return equipment_[index].get();
}

void Greenhouse::set_name(const std::string& value) {
    if (value.empty())
        throw Invalid_argument_exception("name", "must not be empty");
    name_ = value;
}

void Greenhouse::set_length(double value) {
    if (value <= 0.0)
        throw Invalid_argument_exception("length", "must be positive");
    length_ = value;
}

void Greenhouse::set_width(double value) {
    if (value <= 0.0)
        throw Invalid_argument_exception("width", "must be positive");
    width_ = value;
}

void Greenhouse::set_height(double value) {
    if (value <= 0.0)
        throw Invalid_argument_exception("height", "must be positive");
    height_ = value;
}

void Greenhouse::set_max_energy(double value) {
    if (value < 0.0)
        throw Invalid_argument_exception("max_energy", "must be non-negative");
    max_energy_ = value;
    if (current_energy_ > max_energy_) current_energy_ = max_energy_;
}

void Greenhouse::set_current_energy(double value) {
    if (value < 0.0 || value > max_energy_)
        throw Invalid_argument_exception("current_energy", "must be in [0, max_energy]");
    current_energy_ = value;
}