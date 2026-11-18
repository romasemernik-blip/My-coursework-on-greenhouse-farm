#pragma once
#include "Class/Exception/Header/Greenhouse_exception.h"
#include <string>

class Invalid_argument_exception : public Greenhouse_exception {
private:
    std::string argument_name_;
    std::string value_repr_;
public:
    Invalid_argument_exception(const std::string& argument_name,
                               const std::string& value_repr)
        : Greenhouse_exception(
              "Invalid value of argument \"" + argument_name +
              "\": " + value_repr)
        , argument_name_(argument_name)
        , value_repr_(value_repr)
    {}

    const std::string& argument_name() const noexcept { return argument_name_; }
    const std::string& value_repr()    const noexcept { return value_repr_; }
};