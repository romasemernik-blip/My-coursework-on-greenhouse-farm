#pragma once
#include <stdexcept>
#include <string>

class Greenhouse_exception : public std::runtime_error {
public:
    explicit Greenhouse_exception(const std::string& message)
        : std::runtime_error(message) {}

    ~Greenhouse_exception() override = default;
};