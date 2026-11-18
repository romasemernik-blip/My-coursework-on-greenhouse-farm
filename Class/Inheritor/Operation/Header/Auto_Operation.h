#pragma once
#include "Class/Base/Header/Operation.h"

class Auto_operation : public Operation {
private:
    double interval_;
    int    priority_level_;
    double elapsed_time_;

public:
    Auto_operation();
    Auto_operation(const std::string& name,
                   double energy_cost,
                   std::unique_ptr<Human> worker,
                   std::unique_ptr<Equipment> equipment,
                   double interval,
                   int priority_level);
    Auto_operation(const Auto_operation& other) = delete;
    Auto_operation(Auto_operation&& other) noexcept;
    Auto_operation& operator=(const Auto_operation& other) = delete;
    Auto_operation& operator=(Auto_operation&& other) noexcept;
    ~Auto_operation() override = default;

    void execute() override;
    int  get_priority() const override;

    void tick(double dt);
    bool is_ready() const noexcept;
    void reset_timer();

    double get_interval()       const noexcept;
    int    get_priority_level() const noexcept;
    double get_elapsed_time()   const noexcept;

    void set_interval(double value);
    void set_priority_level(int value);
};