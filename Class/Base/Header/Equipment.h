#pragma once
#include <string>
#include <string_view>

class Equipment {
private:
    std::string name_;
    bool        is_broken_ = false;
    bool        is_on_ = false;
    double      energy_requirement_ = 0.0;
    double      wear_level_ = 0.0;

public:
    Equipment() = default;
    Equipment(std::string_view name, double energy_requirement);
    Equipment(const Equipment& other) = default;
    Equipment(Equipment&& other) noexcept = default;
    Equipment& operator=(const Equipment& other) = default;
    Equipment& operator=(Equipment&& other) noexcept = default;
    virtual ~Equipment() = default;

    virtual void turn_on() = 0;
    virtual void turn_off() = 0;
    virtual std::string get_type() const = 0;

    void apply_wear(double amount);
    void repair();

    const std::string& get_name()               const noexcept;
    bool               get_is_broken()          const noexcept;
    bool               get_is_on()              const noexcept;
    double             get_energy_requirement() const noexcept;
    double             get_wear_level()         const noexcept;

    void set_name(std::string_view value);
    void set_is_broken(bool value);
    void set_is_on(bool value);
    void set_energy_requirement(double value);
    void set_wear_level(double value);
};