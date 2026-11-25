#include "Class/Inheritor/Operation/Header/Collect.h"
#include "Class/Exception/Header/All_exception.h"

Collect::Collect(std::string_view name, double energy_cost, std::unique_ptr<Human> worker, std::unique_ptr<Equipment> equipment, std::unique_ptr<Plant> target, double amount)
    : Operation(name, energy_cost, std::move(worker), std::move(equipment)), target_(std::move(target)), amount_(amount) 
{
    if (!target_) throw Invalid_argument_exception("target", "must not be null");
    if (amount < 0.0) throw Invalid_argument_exception("amount", "must be non-negative");
}

void Collect::execute() {
    if (!target_) throw Invalid_argument_exception("target", "is not set");
    if (target_->get_is_dead()) throw Plant_dead_exception(target_->get_type(), target_->get_id());
    if (!target_->get_is_mature()) throw Invalid_argument_exception("target", "plant is not mature");
}

int Collect::get_priority() const { return 2; }

Plant*       Collect::get_target()       noexcept { return target_.get(); }
const Plant* Collect::get_target() const noexcept { return target_.get(); }
double       Collect::get_amount() const noexcept { return amount_; }

void Collect::set_target(std::unique_ptr<Plant> target) {
    if (!target) throw Invalid_argument_exception("target", "must not be null");
    target_ = std::move(target);
}

void Collect::set_amount(double value) {
    if (value < 0.0) throw Invalid_argument_exception("amount", "must be non-negative");
    amount_ = value;
}