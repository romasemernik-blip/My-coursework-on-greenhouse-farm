#pragma once
#include <string>
#include <string_view>
#include <memory>
#include "Class/Base/Header/Human.h"
#include "Class/Base/Header/Equipment.h"

class Operation {
private:
    std::string name_;
    double energy_cost_ = 0.0;
    std::unique_ptr<Human> worker_;
    std::unique_ptr<Equipment> equipment_;

public:
    Operation() = default;
    Operation(std::string_view name, double energy_cost, std::unique_ptr<Human> worker, std::unique_ptr<Equipment> equipment);
    Operation(const Operation& other) = delete;
    Operation(Operation&& other) noexcept = default;
    Operation& operator=(const Operation& other) = delete;
    Operation& operator=(Operation&& other) noexcept = default;
    virtual ~Operation() = default;

    virtual void execute() = 0;
    virtual int get_priority() const = 0;

    const std::string& get_name()        const noexcept;
    double             get_energy_cost() const noexcept;
    Human*             get_worker()      noexcept;
    const Human*       get_worker()      const noexcept;
    Equipment*         get_equipment()   noexcept;
    const Equipment*   get_equipment()   const noexcept;

    void set_name(std::string_view value);
    void set_energy_cost(double value);
    void set_worker(std::unique_ptr<Human> worker);
    void set_equipment(std::unique_ptr<Equipment> equipment);
};