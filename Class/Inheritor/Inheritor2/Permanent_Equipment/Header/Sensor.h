#pragma once
#include "Class/Inheritor/Equipment/Header/Permanent_Equipment.h"
#include <string>
#include <string_view>

class Sensor : public Permanent_equipment {
private:
    double last_value_ = 0.0;
    std::string units_;
    double min_value_ = -100.0;
    double max_value_ = 100.0;

public:
    Sensor() = default;
    Sensor(std::string_view name, double energy_requirement, std::string_view units, double min_value, double max_value);
    Sensor(const Sensor& other) = default;
    Sensor(Sensor&& other) noexcept = default;
    Sensor& operator=(const Sensor& other) = default;
    Sensor& operator=(Sensor&& other) noexcept = default;
    ~Sensor() override = default;

    virtual double read() = 0;
    virtual std::string get_measured_parameter() const = 0;

    double             get_last_value() const noexcept;
    const std::string& get_units()      const noexcept;
    double             get_min_value()  const noexcept;
    double             get_max_value()  const noexcept;

    void set_last_value(double value);
    void set_units(std::string_view value);
    void set_min_value(double value);
    void set_max_value(double value);
};