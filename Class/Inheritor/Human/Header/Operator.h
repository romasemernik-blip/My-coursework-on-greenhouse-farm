#pragma once
#include "Class/Base/Header/Human.h"
#include <string>
#include <string_view>

class Operator : public Human {
private:
    std::string shift_ = "Day";             
    int access_level_ = 1;              
    bool is_on_duty_ = false;               

public:
    Operator() = default;
    Operator(std::string_view first_name, std::string_view last_name, std::string_view middle_name, std::size_t birth_date, std::string_view gender, std::string_view shift, int access_level);
    Operator(const Operator& other) = delete;
    Operator(Operator&& other) noexcept = default;
    Operator& operator=(const Operator& other) = delete;
    Operator& operator=(Operator&& other) noexcept = default;
    ~Operator() override = default;

    std::string get_role() const override;

    const std::string& get_shift() const noexcept;
    int get_access_level() const noexcept;
    bool get_is_on_duty() const noexcept;

    void set_shift(std::string_view value);
    void set_access_level(int value);
    void set_is_on_duty(bool value);

    void toggle_duty();
};