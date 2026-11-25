#include "Class/Inheritor/Operation/Header/Auto_Operation.h"
#include "Class/Exception/Header/All_exception.h"

Auto_operation::Auto_operation(std::string_view name, double energy_cost, std::unique_ptr<Human> worker, std::unique_ptr<Equipment> equipment, double interval, int priority_level, std::function<void()> action)
    : Operation(name, energy_cost, std::move(worker), std::move(equipment)), 
      interval_(interval), 
      priority_level_(priority_level), 
      action_(std::move(action)) 
{
    if (interval <= 0.0) 
        throw Invalid_argument_exception("interval", "must be positive");
}

void Auto_operation::execute() {
    if (action_) {
        action_();
    }
    reset_timer();
}

int Auto_operation::get_priority() const { return priority_level_; }

void Auto_operation::tick(double dt) {
    if (dt < 0.0) throw Invalid_argument_exception("dt", "must be non-negative");
    elapsed_time_ += dt;
}

bool Auto_operation::is_ready() const noexcept { return elapsed_time_ >= interval_; }
void Auto_operation::reset_timer() { elapsed_time_ = 0.0; }

double Auto_operation::get_interval()       const noexcept { return interval_; }
int    Auto_operation::get_priority_level() const noexcept { return priority_level_; }
double Auto_operation::get_elapsed_time()   const noexcept { return elapsed_time_; }

void Auto_operation::set_interval(double value) {
    if (value <= 0.0) throw Invalid_argument_exception("interval", "must be positive");
    interval_ = value;
}

void Auto_operation::set_priority_level(int value) { priority_level_ = value; }
void Auto_operation::set_action(std::function<void()> action) { action_ = std::move(action); }