#pragma once
#include "Class/Exception/Header/Greenhouse_exception.h"
#include <cstddef>
#include <sstream>

class Bed_not_found_exception : public Greenhouse_exception {
private:
    std::size_t bed_index_;
public:
    explicit Bed_not_found_exception(std::size_t bed_index)
        : Greenhouse_exception(make_message(bed_index))
        , bed_index_(bed_index)
    {}

    std::size_t bed_index() const noexcept { return bed_index_; }

private:
    static std::string make_message(std::size_t idx) {
        std::ostringstream os;
        os << "Bed with index " << idx << " not found";
        return os.str();
    }
};