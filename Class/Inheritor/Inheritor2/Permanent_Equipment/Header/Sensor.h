#pragma once
#include "Class/Inheritor/Equipment/Header/Permanent_Equipment.h"

class Sensor : public Permanent_equipment {
protected:
    double last_value_;
    std::string units_;
    double min_value_;
    double max_value_;

public:
    Sensor();
    Sensor(const std::string& name,
           double energy_requirement,
           const std::string& units,
           double min_value,
           double max_value);
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
    void set_units(const std::string& value);
    void set_min_value(double value);
    void set_max_value(double value);
};