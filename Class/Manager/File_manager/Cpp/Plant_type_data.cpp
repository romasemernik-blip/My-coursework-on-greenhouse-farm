#include "Class/Manager/File_manager/Header/Plant_type_data.h"
#include "Class/Exception/Header/All_exception.h"
#include "Class/Config/Header/Config.h"

Plant_type_data::Plant_type_data()
    : optimal_temperature_(Config::DEFAULT_OPTIMAL_TEMPERATURE)
    , optimal_humidity_(Config::DEFAULT_OPTIMAL_HUMIDITY)
    , water_requirement_(Config::DEFAULT_WATER_REQUIREMENT)
    , growth_speed_(Config::DEFAULT_GROWTH_SPEED)
{}

Plant_type_data::Plant_type_data(const std::string& type_name,
                                 double optimal_temperature,
                                 double optimal_humidity,
                                 double water_requirement,
                                 double growth_speed)
    : type_name_(type_name)
    , optimal_temperature_(optimal_temperature)
    , optimal_humidity_(optimal_humidity)
    , water_requirement_(water_requirement)
    , growth_speed_(growth_speed)
{
    if (type_name.empty())
        throw Invalid_argument_exception("type_name", "must not be empty");
    if (optimal_humidity < Config::MIN_HUMIDITY || optimal_humidity > Config::MAX_HUMIDITY)
        throw Invalid_argument_exception("optimal_humidity", "must be in [0, 100]");
    if (water_requirement < 0.0)
        throw Invalid_argument_exception("water_requirement", "must be non-negative");
    if (growth_speed <= 0.0)
        throw Invalid_argument_exception("growth_speed", "must be positive");
}

const std::string& Plant_type_data::get_type_name() const noexcept { return type_name_; }
double Plant_type_data::get_optimal_temperature() const noexcept { return optimal_temperature_; }
double Plant_type_data::get_optimal_humidity() const noexcept { return optimal_humidity_; }
double Plant_type_data::get_water_requirement() const noexcept { return water_requirement_; }
double Plant_type_data::get_growth_speed() const noexcept { return growth_speed_; }

void Plant_type_data::set_type_name(const std::string& value) {
    if (value.empty())
        throw Invalid_argument_exception("type_name", "must not be empty");
    type_name_ = value;
}

void Plant_type_data::set_optimal_temperature(double value) { 
    optimal_temperature_ = value; 
}

void Plant_type_data::set_optimal_humidity(double value) {
    if (value < Config::MIN_HUMIDITY || value > Config::MAX_HUMIDITY)
        throw Invalid_argument_exception("optimal_humidity", "must be in [0, 100]");
    optimal_humidity_ = value;
}

void Plant_type_data::set_water_requirement(double value) {
    if (value < 0.0)
        throw Invalid_argument_exception("water_requirement", "must be non-negative");
    water_requirement_ = value;
}

void Plant_type_data::set_growth_speed(double value) {
    if (value <= 0.0)
        throw Invalid_argument_exception("growth_speed", "must be positive");
    growth_speed_ = value;
}