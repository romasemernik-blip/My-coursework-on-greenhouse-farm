#pragma once
#include <stdexcept>
#include <string>

class Greenhouse_exception : public std::runtime_error {
public:
    using std::runtime_error::runtime_error; 
    ~Greenhouse_exception() override = default;
};