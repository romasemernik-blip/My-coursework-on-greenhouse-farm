#include "Class/Base/Header/Storage.h"
#include "Class/Exception/Header/All_exception.h"

Storage::Storage(double capacity) : capacity_(capacity) {
    if (capacity < 0.0) throw Invalid_argument_exception("capacity", "must be non-negative");
}

void Storage::add(std::string_view crop, double amount) {
    if (crop.empty()) throw Invalid_argument_exception("crop", "must not be empty");
    if (amount < 0.0) throw Invalid_argument_exception("amount", "must be non-negative");
    if (amount > get_free_space()) throw Storage_overflow_exception(amount, get_free_space());

    contents_[std::string(crop)] += amount;
    current_load_ += amount;
}

void Storage::remove(std::string_view crop, double amount) {
    if (amount < 0.0) throw Invalid_argument_exception("amount", "must be non-negative");

    auto it = contents_.find(crop);
    if (it == contents_.end() || it->second < amount)
        throw Invalid_argument_exception("amount", "not enough crop in storage");

    it->second -= amount;
    current_load_ -= amount;

    if (it->second <= 0.00001) {
        contents_.erase(it);
    }
}

double Storage::get_amount(std::string_view crop) const {
    auto it = contents_.find(crop);
    return it != contents_.end() ? it->second : 0.0;
}

void Storage::clear() {
    contents_.clear();
    current_load_ = 0.0;
}

double Storage::get_capacity()     const noexcept { return capacity_; }
double Storage::get_current_load() const noexcept { return current_load_; }

double Storage::get_free_space()   const noexcept {
    double free_space = capacity_ - current_load_;
    return free_space < 0.0 ? 0.0 : free_space;
}

void Storage::set_capacity(double value) {
    if (value < 0.0) throw Invalid_argument_exception("capacity", "must be non-negative");
    if (value < current_load_) throw Invalid_argument_exception("capacity", "cannot be less than current load");
    capacity_ = value;
}