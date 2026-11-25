#include "Class/Base/Header/Schedule.h"
#include "Class/Exception/Header/All_exception.h"
#include "Class/Inheritor/Operation/Header/Auto_Operation.h"
#include <algorithm>

Schedule::Schedule()
    : is_paused_(false)
    , time_manager_(nullptr)
{}

Schedule::Schedule(Time_manager* time_manager)
    : is_paused_(false)
    , time_manager_(time_manager)
{
    if (!time_manager_)
        throw Invalid_argument_exception("time_manager", "must not be null");
}

Schedule::Schedule(Schedule&& other) noexcept = default;
Schedule& Schedule::operator=(Schedule&& other) noexcept = default;

void Schedule::add_operation(std::unique_ptr<Operation> operation) {
    if (!operation)
        throw Invalid_argument_exception("operation", "must not be null");
    operations_.push_back(std::move(operation));
}

std::unique_ptr<Operation> Schedule::remove_operation(std::size_t index) {
    if (index >= operations_.size())
        throw Invalid_argument_exception("index", "operation index out of range");
    auto result = std::move(operations_[index]);
    operations_.remove_at(index);
    return result;
}

void Schedule::clear_operations() {
    operations_.clear();
}

void Schedule::update(double dt) {
    if (is_paused_) return;
    if (dt < 0.0)
        throw Invalid_argument_exception("dt", "must be non-negative");

    for (std::size_t i = 0; i < operations_.size(); ++i) {
        Auto_operation* auto_op = dynamic_cast<Auto_operation*>(operations_[i].get());
        if (auto_op) {
            auto_op->tick(dt);
        }
    }
}

void Schedule::execute_all() {
    if (is_paused_) return;

    std::vector<Operation*> ready_ops;
    for (std::size_t i = 0; i < operations_.size(); ++i) {
        Auto_operation* auto_op = dynamic_cast<Auto_operation*>(operations_[i].get());
        if (auto_op && auto_op->is_ready()) {
            ready_ops.push_back(auto_op);
        } else if (!auto_op) {
            ready_ops.push_back(operations_[i].get());
        }
    }

    std::sort(ready_ops.begin(), ready_ops.end(),
        [](const Operation* a, const Operation* b) {
            return a->get_priority() > b->get_priority();
        });

    for (Operation* op : ready_ops) {
        op->execute();
    }
}

void Schedule::pause()  { is_paused_ = true; }
void Schedule::resume() { is_paused_ = false; }

bool Schedule::get_is_paused() const noexcept { return is_paused_; }
std::size_t Schedule::get_operation_count() const noexcept { return operations_.size(); }

Operation* Schedule::get_operation(std::size_t index) {
    if (index >= operations_.size())
        throw Invalid_argument_exception("index", "operation index out of range");
    return operations_[index].get();
}

const Operation* Schedule::get_operation(std::size_t index) const {
    if (index >= operations_.size())
        throw Invalid_argument_exception("index", "operation index out of range");
    return operations_[index].get();
}

Time_manager* Schedule::get_time_manager() noexcept { return time_manager_; }
const Time_manager* Schedule::get_time_manager() const noexcept { return time_manager_; }

void Schedule::set_time_manager(Time_manager* time_manager) {
    if (!time_manager)
        throw Invalid_argument_exception("time_manager", "must not be null");
    time_manager_ = time_manager;
}