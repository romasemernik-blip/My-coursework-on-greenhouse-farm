#pragma once
#include "Class/Exception/Header/Greenhouse_exception.h"
#include <string>

class Plant_dead_exception : public Greenhouse_exception {
private:
    std::string plant_type_;
    std::string plant_id_;
public:
    Plant_dead_exception(const std::string& plant_type,
                         const std::string& plant_id)
        : Greenhouse_exception(
              "Plant " + plant_type + " (id=" + plant_id + ") is dead")
        , plant_type_(plant_type)
        , plant_id_(plant_id)
    {}

    const std::string& plant_type() const noexcept { return plant_type_; }
    const std::string& plant_id()   const noexcept { return plant_id_; }
};