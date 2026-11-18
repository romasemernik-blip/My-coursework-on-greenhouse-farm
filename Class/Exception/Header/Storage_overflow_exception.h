#pragma once
#include "Class/Exception/Header/Greenhouse_exception.h"
#include <sstream>
#include <iomanip>

class Storage_overflow_exception : public Greenhouse_exception {
private:
    double requested_amount_;
    double free_space_;
public:
    Storage_overflow_exception(double requested_amount, double free_space)
        : Greenhouse_exception(make_message(requested_amount, free_space))
        , requested_amount_(requested_amount)
        , free_space_(free_space)
    {}

    double requested_amount() const noexcept { return requested_amount_; }
    double free_space()       const noexcept { return free_space_; }

private:
    static std::string make_message(double req, double free) {
        std::ostringstream os;
        os << std::fixed << std::setprecision(2)
           << "Storage overflow: attempted to add " << req
           << " kg, free space only " << free << " kg";
        return os.str();
    }
};