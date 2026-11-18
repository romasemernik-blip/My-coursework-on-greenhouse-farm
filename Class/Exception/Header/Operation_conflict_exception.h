#pragma once
#include "Class/Exception/Header/Greenhouse_exception.h"
#include <string>

class Operation_conflict_exception : public Greenhouse_exception {
private:
    std::string first_operation_;
    std::string second_operation_;
public:
    Operation_conflict_exception(const std::string& first,
                                 const std::string& second)
        : Greenhouse_exception(
              "Operation conflict: \"" + first + "\" and \"" + second +
              "\" cannot be executed simultaneously")
        , first_operation_(first)
        , second_operation_(second)
    {}

    const std::string& first_operation()  const noexcept { return first_operation_; }
    const std::string& second_operation() const noexcept { return second_operation_; }
};