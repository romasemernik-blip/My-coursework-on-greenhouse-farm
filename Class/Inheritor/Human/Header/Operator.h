#pragma once
#include "Class/Base/Header/Human.h"
#include <string>

class Operator : public Human {
private:
    std::string shift_;             
    int access_level_;              
    bool is_on_duty_;               

public:
    Operator();
    Operator(const std::string& first_name,
             const std::string& last_name,
             const std::string& middle_name,
             std::size_t birth_date,
             const std::string& gender,
             const std::string& shift,
             int access_level);
    Operator(const Operator& other) = delete;
    Operator(Operator&& other) noexcept;
    Operator& operator=(const Operator& other) = delete;
    Operator& operator=(Operator&& other) noexcept;
    ~Operator() override = default;

    std::string get_role() const override;

    const std::string& get_shift() const noexcept;
    int get_access_level() const noexcept;
    bool get_is_on_duty() const noexcept;

    void set_shift(const std::string& value);
    void set_access_level(int value);
    void set_is_on_duty(bool value);

    void toggle_duty();
};