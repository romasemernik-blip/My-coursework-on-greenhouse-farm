#pragma once
#include "Class/Base/Header/Operation.h"
#include <string_view>
#include <functional>

class Auto_operation : public Operation {
private:
    double interval_ = 1.0;
    int    priority_level_ = 0;
    double elapsed_time_ = 0.0;
    std::function<void()> action_; 

public:
    Auto_operation() = default;
    
    // Конструктор с действием
    Auto_operation(std::string_view name, 
                   double energy_cost, 
                   std::unique_ptr<Human> worker, 
                   std::unique_ptr<Equipment> equipment, 
                   double interval, 
                   int priority_level,
                   std::function<void()> action = nullptr);

    Auto_operation(const Auto_operation& other) = delete;
    Auto_operation(Auto_operation&& other) noexcept = default;
    Auto_operation& operator=(const Auto_operation& other) = delete;
    Auto_operation& operator=(Auto_operation&& other) noexcept = default;
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
    void set_action(std::function<void()> action);
};