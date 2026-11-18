#pragma once
#include "Class/Exception/Header/Greenhouse_exception.h"
#include <string>
#include <sstream>
#include <iomanip>

class Equipment_failure_exception : public Greenhouse_exception {
private:
    std::string equipment_name_;
    double wear_level_;
public:
    Equipment_failure_exception(const std::string& name, double wear_level)
        : Greenhouse_exception(make_message(name, wear_level))
        , equipment_name_(name)
        , wear_level_(wear_level)
    {}

    const std::string& equipment_name() const noexcept { return equipment_name_; }
    double wear_level()                 const noexcept { return wear_level_; }

private:
    static std::string make_message(const std::string& name, double wear) {
        std::ostringstream os;
        os << std::fixed << std::setprecision(1)
           << "Equipment \"" << name << "\" is broken (wear "
           << wear << "%)";
        return os.str();
    }
};