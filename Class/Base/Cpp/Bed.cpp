#include "Class/Base/Header/Bed.h"
#include "Class/Exception/Header/All_exception.h"

Bed::Bed()
    : numb_row_(0)
    , numb_crop_row_(0)
    , soil_ph_(7.0)
    , moisture_(50.0)
    , readiness_(0.0)
    , co2_level_(400.0)
    , length_(0.0)
    , width_(0.0)
{}

Bed::Bed(int numb_row,
         int numb_crop_row,
         double length,
         double width)
    : numb_row_(numb_row)
    , numb_crop_row_(numb_crop_row)
    , soil_ph_(7.0)
    , moisture_(50.0)
    , readiness_(0.0)
    , co2_level_(400.0)
    , length_(length)
    , width_(width)
{
    if (numb_row < 0)
        throw Invalid_argument_exception("numb_row", "must be non-negative");
    if (numb_crop_row < 0)
        throw Invalid_argument_exception("numb_crop_row", "must be non-negative");
    if (length < 0.0)
        throw Invalid_argument_exception("length", "must be non-negative");
    if (width < 0.0)
        throw Invalid_argument_exception("width", "must be non-negative");
}

Bed::Bed(Bed&& other) noexcept = default;
Bed& Bed::operator=(Bed&& other) noexcept = default;

double Bed::get_area() const noexcept {
    return length_ * width_;
}

void Bed::add_plant(std::unique_ptr<Plant> plant) {
    if (!plant)
        throw Invalid_argument_exception("plant", "must not be null");
    plants_.push_back(std::move(plant));
}

void Bed::add_equipment(std::unique_ptr<Equipment> equipment) {
    if (!equipment)
        throw Invalid_argument_exception("equipment", "must not be null");
    equipments_.push_back(std::move(equipment));
}

std::unique_ptr<Plant> Bed::remove_plant(std::size_t index) {
    if (index >= plants_.size())
        throw Invalid_argument_exception("index", "plant index out of range");
    auto result = std::move(plants_[index]);
    plants_.remove_at(index);
    return result;
}

std::unique_ptr<Equipment> Bed::remove_equipment(std::size_t index) {
    if (index >= equipments_.size())
        throw Invalid_argument_exception("index", "equipment index out of range");
    auto result = std::move(equipments_[index]);
    equipments_.remove_at(index);
    return result;
}

void Bed::clear_plants()     { plants_.clear(); }
void Bed::clear_equipments() { equipments_.clear(); }

int    Bed::get_numb_row()        const noexcept { return numb_row_; }
int    Bed::get_numb_crop_row()   const noexcept { return numb_crop_row_; }
double Bed::get_soil_ph()         const noexcept { return soil_ph_; }
double Bed::get_moisture()        const noexcept { return moisture_; }
double Bed::get_readiness()       const noexcept { return readiness_; }
double Bed::get_co2_level()       const noexcept { return co2_level_; }
double Bed::get_length()          const noexcept { return length_; }
double Bed::get_width()           const noexcept { return width_; }
std::size_t Bed::get_plant_count()     const noexcept { return plants_.size(); }
std::size_t Bed::get_equipment_count() const noexcept { return equipments_.size(); }

Plant* Bed::get_plant(std::size_t index) {
    if (index >= plants_.size())
        throw Invalid_argument_exception("index", "plant index out of range");
    return plants_[index].get();
}

const Plant* Bed::get_plant(std::size_t index) const {
    if (index >= plants_.size())
        throw Invalid_argument_exception("index", "plant index out of range");
    return plants_[index].get();
}

Equipment* Bed::get_equipment(std::size_t index) {
    if (index >= equipments_.size())
        throw Invalid_argument_exception("index", "equipment index out of range");
    return equipments_[index].get();
}

const Equipment* Bed::get_equipment(std::size_t index) const {
    if (index >= equipments_.size())
        throw Invalid_argument_exception("index", "equipment index out of range");
    return equipments_[index].get();
}

void Bed::set_numb_row(int value) {
    if (value < 0)
        throw Invalid_argument_exception("numb_row", "must be non-negative");
    numb_row_ = value;
}

void Bed::set_numb_crop_row(int value) {
    if (value < 0)
        throw Invalid_argument_exception("numb_crop_row", "must be non-negative");
    numb_crop_row_ = value;
}

void Bed::set_soil_ph(double value) {
    if (value < 0.0 || value > 14.0)
        throw Invalid_argument_exception("soil_ph", "must be in [0, 14]");
    soil_ph_ = value;
}

void Bed::set_moisture(double value) {
    if (value < 0.0 || value > 100.0)
        throw Invalid_argument_exception("moisture", "must be in [0, 100]");
    moisture_ = value;
}

void Bed::set_readiness(double value) {
    if (value < 0.0 || value > 100.0)
        throw Invalid_argument_exception("readiness", "must be in [0, 100]");
    readiness_ = value;
}

void Bed::set_co2_level(double value) {
    if (value < 0.0)
        throw Invalid_argument_exception("co2_level", "must be non-negative");
    co2_level_ = value;
}

void Bed::set_length(double value) {
    if (value < 0.0)
        throw Invalid_argument_exception("length", "must be non-negative");
    length_ = value;
}

void Bed::set_width(double value) {
    if (value < 0.0)
        throw Invalid_argument_exception("width", "must be non-negative");
    width_ = value;
}

void Bed::update(double dt, bool is_light_on) {
    for (std::size_t i = 0; i < plants_.size(); ++i) {
        plants_[i]->update(dt, moisture_, soil_ph_, is_light_on);
    }
}