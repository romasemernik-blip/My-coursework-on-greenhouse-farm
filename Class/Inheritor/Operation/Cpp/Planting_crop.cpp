#include "Class/Inheritor/Operation/Header/Planting_crop.h"
#include "Class/Exception/Header/All_exception.h"

Planting_crop::Planting_crop() = default;

Planting_crop::Planting_crop(const std::string& name,
                             double energy_cost,
                             std::unique_ptr<Human> worker,
                             std::unique_ptr<Equipment> equipment,
                             std::unique_ptr<Plant> plant)
    : Operation(name, energy_cost, std::move(worker), std::move(equipment))
    , plant_(std::move(plant))
{
    if (!plant_)
        throw Invalid_argument_exception("plant", "must not be null");
}

Planting_crop::Planting_crop(Planting_crop&& other) noexcept = default;
Planting_crop& Planting_crop::operator=(Planting_crop&& other) noexcept = default;

void Planting_crop::execute() {
    if (!plant_)
        throw Invalid_argument_exception("plant", "is not set");
}

int Planting_crop::get_priority() const { return 1; }

Plant*       Planting_crop::get_plant()       noexcept { return plant_.get(); }
const Plant* Planting_crop::get_plant() const noexcept { return plant_.get(); }

void Planting_crop::set_plant(std::unique_ptr<Plant> plant) {
    if (!plant)
        throw Invalid_argument_exception("plant", "must not be null");
    plant_ = std::move(plant);
}