#include "Class/Inheritor/Operation/Header/Auto_Operation.h"
#include "Class/Exception/Header/All_exception.h"

Auto_operation::Auto_operation()
    : interval_(1.0)
    , priority_level_(0)
    , elapsed_time_(0.0)
{}

Auto_operation::Auto_operation(const std::string& name,
                               double energy_cost,
                               std::unique_ptr<Human> worker,
                               std::unique_ptr<Equipment> equipment,
                               double interval,
                               int priority_level)
    : Operation(name, energy_cost, std::move(worker), std::move(equipment))
    , interval_(interval)
    , priority_level_(priority_level)
    , elapsed_time_(0.0)
{
    if (interval <= 0.0)
        throw Invalid_argument_exception("interval", "must be positive");
}

Auto_operation::Auto_operation(Auto_operation&& other) noexcept = default;
Auto_operation& Auto_operation::operator=(Auto_operation&& other) noexcept = default;

void Auto_operation::execute() {
    reset_timer();
}

int Auto_operation::get_priority() const { return priority_level_; }

void Auto_operation::tick(double dt) {
    if (dt < 0.0)
        throw Invalid_argument_exception("dt", "must be non-negative");
    elapsed_time_ += dt;
}

bool Auto_operation::is_ready() const noexcept {
    return elapsed_time_ >= interval_;
}

void Auto_operation::reset_timer() {
    elapsed_time_ = 0.0;
}

double Auto_operation::get_interval()       const noexcept { return interval_; }
int    Auto_operation::get_priority_level() const noexcept { return priority_level_; }
double Auto_operation::get_elapsed_time()   const noexcept { return elapsed_time_; }

void Auto_operation::set_interval(double value) {
    if (value <= 0.0)
        throw Invalid_argument_exception("interval", "must be positive");
    interval_ = value;
}

void Auto_operation::set_priority_level(int value) {
    priority_level_ = value;
}