#pragma once
#include <string>

class Plant_type_data {
private:
    std::string type_name_;
    double optimal_temperature_;
    double optimal_humidity_;
    double water_requirement_;
    double growth_speed_;

public:
    Plant_type_data();
    Plant_type_data(const std::string& type_name,
                    double optimal_temperature,
                    double optimal_humidity,
                    double water_requirement,
                    double growth_speed);

    const std::string& get_type_name() const noexcept;
    double get_optimal_temperature() const noexcept;
    double get_optimal_humidity() const noexcept;
    double get_water_requirement() const noexcept;
    double get_growth_speed() const noexcept;

    void set_type_name(const std::string& value);
    void set_optimal_temperature(double value);
    void set_optimal_humidity(double value);
    void set_water_requirement(double value);
    void set_growth_speed(double value);
};