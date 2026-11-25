#include "Class/Base/Header/Plant.h"
#include "Class/Exception/Header/All_exception.h"

Plant::Plant()
    : height_(0.0)
    , water_requirement_(0.0)
    , current_health_(100.0)
    , optimal_temperature_(22.0)
    , optimal_humidity_(60.0)
    , stage_(Growth_stage::Seed)
    , stage_progress_(0.0)
    , stage_duration_hours_(24.0)
{}

Plant::Plant(const std::string& id,
             const std::string& type,
             double height,
             double water_requirement,
             double optimal_temperature,
             double optimal_humidity)
    : id_(id)
    , type_(type)
    , height_(height)
    , water_requirement_(water_requirement)
    , current_health_(100.0)
    , optimal_temperature_(optimal_temperature)
    , optimal_humidity_(optimal_humidity)
    , stage_(Growth_stage::Seed)
    , stage_progress_(0.0)
    , stage_duration_hours_(24.0)
{
    if (height < 0.0)
        throw Invalid_argument_exception("height", "must be non-negative");
    if (water_requirement < 0.0)
        throw Invalid_argument_exception("water_requirement", "must be non-negative");
}

void Plant::grow(double dt) {
    if (get_is_dead()) return;
    advance_stage(dt);
    height_ += 0.01 * dt;
}

void Plant::advance_stage(double dt) {
    if (dt < 0.0)
        throw Invalid_argument_exception("dt", "must be non-negative");
    if (stage_ == Growth_stage::Dead) return;

    stage_progress_ += dt / stage_duration_hours_;

    while (stage_progress_ >= 1.0) {
        stage_progress_ -= 1.0;
        switch (stage_) {
        case Growth_stage::Seed:
            stage_ = Growth_stage::Vegetative;
            stage_duration_hours_ = 48.0;
            break;
        case Growth_stage::Vegetative:
            stage_ = Growth_stage::Flowering;
            stage_duration_hours_ = 36.0;
            break;
        case Growth_stage::Flowering:
            stage_ = Growth_stage::Fruiting;
            stage_duration_hours_ = 72.0;
            break;
        case Growth_stage::Fruiting:
            stage_ = Growth_stage::Dead;
            stage_progress_ = 0.0;
            return;
        case Growth_stage::Dead:
            return;
        }
    }
}

void Plant::kill() {
    stage_ = Growth_stage::Dead;
    stage_progress_ = 0.0;
}

void Plant::damage(double amount) {
    if (amount < 0.0)
        throw Invalid_argument_exception("amount", "must be non-negative");
    current_health_ -= amount;
    if (current_health_ <= 0.0) {
        current_health_ = 0.0;
        kill();
    }
}

void Plant::heal(double amount) {
    if (amount < 0.0)
        throw Invalid_argument_exception("amount", "must be non-negative");
    current_health_ += amount;
    if (current_health_ > 100.0) current_health_ = 100.0;
}

const std::string& Plant::get_id()                  const noexcept { return id_; }
const std::string& Plant::get_type()                const noexcept { return type_; }
double             Plant::get_height()              const noexcept { return height_; }
double             Plant::get_water_requirement()   const noexcept { return water_requirement_; }
double             Plant::get_current_health()      const noexcept { return current_health_; }
double             Plant::get_optimal_temperature() const noexcept { return optimal_temperature_; }
double             Plant::get_optimal_humidity()    const noexcept { return optimal_humidity_; }
Growth_stage       Plant::get_stage()               const noexcept { return stage_; }
double             Plant::get_stage_progress()      const noexcept { return stage_progress_; }
double             Plant::get_stage_duration()      const noexcept { return stage_duration_hours_; }
bool               Plant::get_is_dead()             const noexcept { return stage_ == Growth_stage::Dead; }
bool               Plant::get_is_mature()           const noexcept { return stage_ == Growth_stage::Fruiting; }

void Plant::set_id(const std::string& value) {
    id_ = value;
}

void Plant::set_type(const std::string& value) {
    if (value.empty())
        throw Invalid_argument_exception("type", "must not be empty");
    type_ = value;
}

void Plant::set_height(double value) {
    if (value < 0.0)
        throw Invalid_argument_exception("height", "must be non-negative");
    height_ = value;
}

void Plant::set_water_requirement(double value) {
    if (value < 0.0)
        throw Invalid_argument_exception("water_requirement", "must be non-negative");
    water_requirement_ = value;
}

void Plant::set_current_health(double value) {
    if (value < 0.0 || value > 100.0)
        throw Invalid_argument_exception("current_health", "must be in [0, 100]");
    current_health_ = value;
}

void Plant::set_optimal_temperature(double value) {
    optimal_temperature_ = value;
}

void Plant::set_optimal_humidity(double value) {
    if (value < 0.0 || value > 100.0)
        throw Invalid_argument_exception("optimal_humidity", "must be in [0, 100]");
    optimal_humidity_ = value;
}

void Plant::set_stage(Growth_stage value) {
    stage_ = value;
}

void Plant::set_stage_progress(double value) {
    if (value < 0.0 || value > 1.0)
        throw Invalid_argument_exception("stage_progress", "must be in [0, 1]");
    stage_progress_ = value;
}

void Plant::set_stage_duration(double value) {
    if (value <= 0.0)
        throw Invalid_argument_exception("stage_duration", "must be positive");
    stage_duration_hours_ = value;
}

double Plant::get_hours_until_mature() const noexcept {
    if (stage_ == Growth_stage::Fruiting || stage_ == Growth_stage::Dead) return 0.0;
    
    double remaining = (1.0 - stage_progress_) * stage_duration_hours_;
    
    switch (stage_) {
    case Growth_stage::Seed:
        remaining += 48.0 + 36.0 + 72.0;
        break;
    case Growth_stage::Vegetative:
        remaining += 36.0 + 72.0;
        break;
    case Growth_stage::Flowering:
        remaining += 72.0;
        break;
    default: break;
    }
    return remaining;
}