#pragma once
#include "Class/Base/Header/Operation.h"
#include "Class/Base/Header/Plant.h"
#include <memory>
#include <string_view>

class Planting_crop : public Operation {
private:
    std::unique_ptr<Plant> plant_;

public:
    Planting_crop() = default;
    Planting_crop(std::string_view name, double energy_cost, std::unique_ptr<Human> worker, std::unique_ptr<Equipment> equipment, std::unique_ptr<Plant> plant);
    Planting_crop(const Planting_crop& other) = delete;
    Planting_crop(Planting_crop&& other) noexcept = default;
    Planting_crop& operator=(const Planting_crop& other) = delete;
    Planting_crop& operator=(Planting_crop&& other) noexcept = default;
    ~Planting_crop() override = default;

    void execute() override;
    int  get_priority() const override;

    Plant*       get_plant()       noexcept;
    const Plant* get_plant() const noexcept;

    void set_plant(std::unique_ptr<Plant> plant);
};