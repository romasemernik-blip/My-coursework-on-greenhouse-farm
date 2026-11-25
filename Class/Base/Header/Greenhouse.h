#pragma once
#include <string>
#include <string_view>
#include <memory>
#include "Class/Template/Collection.h"
#include "Class/Base/Header/Bed.h"
#include "Class/Base/Header/Equipment.h"

class Greenhouse {
private:
    std::string name_;
    double length_ = 0.0;
    double width_ = 0.0;
    double height_ = 0.0;
    double max_energy_ = 0.0;
    double current_energy_ = 0.0;
    Collection<std::unique_ptr<Bed>> beds_;
    Collection<std::unique_ptr<Equipment>> equipment_;

public:
    Greenhouse() = default;
    Greenhouse(std::string_view name, double length, double width, double height, double max_energy);
    Greenhouse(const Greenhouse& other) = delete;
    Greenhouse(Greenhouse&& other) noexcept = default;
    Greenhouse& operator=(const Greenhouse& other) = delete;
    Greenhouse& operator=(Greenhouse&& other) noexcept = default;
    ~Greenhouse() = default;

    double get_total_area() const noexcept;
    double get_free_area()  const noexcept;
    double get_used_area()  const noexcept;

    void add_bed(std::unique_ptr<Bed> bed);
    void add_equipment(std::unique_ptr<Equipment> equipment);
    std::unique_ptr<Bed> remove_bed(std::size_t index);
    std::unique_ptr<Equipment> remove_equipment(std::size_t index);
    void clear_beds();
    void clear_equipment();

    void consume_energy(double amount);
    void restore_energy(double amount);

    const std::string& get_name()          const noexcept;
    double             get_length()        const noexcept;
    double             get_width()         const noexcept;
    double             get_height()        const noexcept;
    double             get_max_energy()    const noexcept;
    double             get_current_energy() const noexcept;
    std::size_t        get_bed_count()     const noexcept;
    std::size_t        get_equipment_count() const noexcept;
    
    Bed*               get_bed(std::size_t index);
    const Bed*         get_bed(std::size_t index) const;
    Equipment*         get_equipment(std::size_t index);
    const Equipment*   get_equipment(std::size_t index) const;

    const Collection<std::unique_ptr<Bed>>& get_beds() const noexcept;
    const Collection<std::unique_ptr<Equipment>>& get_equipment_collection() const noexcept;

    void set_name(std::string_view value);
    void set_length(double value);
    void set_width(double value);
    void set_height(double value);
    void set_max_energy(double value);
    void set_current_energy(double value);
    void update(double dt, bool global_light_on) const;
};