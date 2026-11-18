#pragma once
#include "Class/Base/Header/Operation.h"
#include "Class/Base/Header/Plant.h"
#include <memory>

class Collect : public Operation {
private:
    std::unique_ptr<Plant> target_;
    double amount_;

public:
    Collect();
    Collect(const std::string& name,
            double energy_cost,
            std::unique_ptr<Human> worker,
            std::unique_ptr<Equipment> equipment,
            std::unique_ptr<Plant> target,
            double amount);
    Collect(const Collect& other) = delete;
    Collect(Collect&& other) noexcept;
    Collect& operator=(const Collect& other) = delete;
    Collect& operator=(Collect&& other) noexcept;
    ~Collect() override = default;

    void execute() override;
    int  get_priority() const override;

    Plant*       get_target()       noexcept;
    const Plant* get_target() const noexcept;
    double       get_amount() const noexcept;

    void set_target(std::unique_ptr<Plant> target);
    void set_amount(double value);
};