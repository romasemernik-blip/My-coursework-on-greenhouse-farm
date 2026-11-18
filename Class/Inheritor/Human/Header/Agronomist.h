#pragma once
#include "Class/Base/Header/Human.h"
#include "Class/Template/Collection.h"
#include <memory>

class Bed;

class Agronomist : public Human {
private:
    int start_year_;
    int experience_years_;
    Collection<std::unique_ptr<Bed>> assigned_beds_;

public:
    Agronomist();
    Agronomist(const std::string& first_name,
               const std::string& last_name,
               const std::string& middle_name,
               std::size_t birth_date,
               const std::string& gender,
               int start_year,
               int experience_years);
    Agronomist(const Agronomist& other) = delete;
    Agronomist(Agronomist&& other) noexcept;
    Agronomist& operator=(const Agronomist& other) = delete;
    Agronomist& operator=(Agronomist&& other) noexcept;
    ~Agronomist() override = default;

    std::string get_role() const override;

    int get_start_year()       const noexcept;
    int get_experience_years() const noexcept;
    std::size_t get_bed_count() const noexcept;

    void set_start_year(int value);
    void set_experience_years(int value);

    void add_bed(std::unique_ptr<Bed> bed);
    void clear_beds();
};