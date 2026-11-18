#pragma once
#include "Class/Exception/Header/Greenhouse_exception.h"
#include <cstddef>
#include <sstream>

class Greenhouse_full_exception : public Greenhouse_exception {
private:
    std::size_t required_area_;
    std::size_t available_area_;
public:
    Greenhouse_full_exception(std::size_t required_area,
                              std::size_t available_area)
        : Greenhouse_exception(make_message(required_area, available_area))
        , required_area_(required_area)
        , available_area_(available_area)
    {}

    std::size_t required_area()  const noexcept { return required_area_; }
    std::size_t available_area() const noexcept { return available_area_; }

private:
    static std::string make_message(std::size_t req, std::size_t avail) {
        std::ostringstream os;
        os << "Not enough space in greenhouse: required "
           << req << " m², available " << avail << " m²";
        return os.str();
    }
};