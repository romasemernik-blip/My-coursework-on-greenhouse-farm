#pragma once
#include <string>

class Equipment {
protected:
    std::string name_;
    bool        is_broken_;
    bool        is_on_;
    double      energy_requirement_;
    double      wear_level_;

public:
    Equipment();
    Equipment(const std::string& name, double energy_requirement);
    Equipment(const Equipment& other);
    Equipment(Equipment&& other) noexcept;
    Equipment& operator=(const Equipment& other);
    Equipment& operator=(Equipment&& other) noexcept;
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

    void set_name(const std::string& value);
    void set_is_broken(bool value);
    void set_is_on(bool value);
    void set_energy_requirement(double value);
    void set_wear_level(double value);
};