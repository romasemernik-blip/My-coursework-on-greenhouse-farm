#pragma once
#include "Class/Exception/Header/Greenhouse_exception.h"
#include <string>

class Sensor_read_exception : public Greenhouse_exception {
private:
    std::string sensor_name_;
    std::string reason_;
public:
    Sensor_read_exception(const std::string& sensor_name,
                          const std::string& reason)
        : Greenhouse_exception("Sensor \"" + sensor_name + "\": " + reason)
        , sensor_name_(sensor_name)
        , reason_(reason)
    {}

    const std::string& sensor_name() const noexcept { return sensor_name_; }
    const std::string& reason()      const noexcept { return reason_; }
};