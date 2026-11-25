#pragma once
#include <string>
#include <memory>
#include "Class/Template/Collection.h"
#include "Class/Base/Header/Plant.h"
#include "Class/Base/Header/Equipment.h"

class Bed {
private:
    int numb_row_ = 0;
    int numb_crop_row_ = 0;
    double soil_ph_ = 7.0;
    double moisture_ = 50.0;
    double readiness_ = 0.0;
    double co2_level_ = 400.0;
    double length_ = 0.0;
    double width_ = 0.0;
    Collection<std::unique_ptr<Plant>> plants_;
    Collection<std::unique_ptr<Equipment>> equipments_;

public:
    Bed() = default;
    Bed(int numb_row, int numb_crop_row, double length, double width);
    Bed(const Bed& other) = delete;
    Bed(Bed&& other) noexcept = default;
    Bed& operator=(const Bed& other) = delete;
    Bed& operator=(Bed&& other) noexcept = default;
    ~Bed() = default;

    double get_area() const noexcept;
    void add_plant(std::unique_ptr<Plant> plant);
    void add_equipment(std::unique_ptr<Equipment> equipment);
    std::unique_ptr<Plant> remove_plant(std::size_t index);
    std::unique_ptr<Equipment> remove_equipment(std::size_t index);
    void clear_plants();
    void clear_equipments();

    int                get_numb_row()        const noexcept;
    int                get_numb_crop_row()   const noexcept;
    double             get_soil_ph()         const noexcept;
    double             get_moisture()        const noexcept;
    double             get_readiness()       const noexcept;
    double             get_co2_level()       const noexcept;
    double             get_length()          const noexcept;
    double             get_width()           const noexcept;
    std::size_t        get_plant_count()     const noexcept;
    std::size_t        get_equipment_count() const noexcept;
    Plant*             get_plant(std::size_t index);
    const Plant*       get_plant(std::size_t index) const;
    Equipment*         get_equipment(std::size_t index);
    const Equipment*   get_equipment(std::size_t index) const;

    void set_numb_row(int value);
    void set_numb_crop_row(int value);
    void set_soil_ph(double value);
    void set_moisture(double value);
    void set_readiness(double value);
    void set_co2_level(double value);
    void set_length(double value);
    void set_width(double value);
    void update (double dt, bool is_light_on) const;
};