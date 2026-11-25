#pragma once
#include "Class/Base/Header/Human.h"
#include "Class/Base/Header/Bed.h"
#include "Class/Template/Collection.h"
#include <memory>
#include <string_view>

class Agronomist : public Human {
private:
    int start_year_ = 0;
    int experience_years_ = 0;
    Collection<std::unique_ptr<Bed>> assigned_beds_;

public:
    Agronomist() = default;
    Agronomist(std::string_view first_name, std::string_view last_name, std::string_view middle_name, std::size_t birth_date, std::string_view gender, int start_year, int experience_years);
    Agronomist(const Agronomist& other) = delete;
    Agronomist(Agronomist&& other) noexcept = default;
    Agronomist& operator=(const Agronomist& other) = delete;
    Agronomist& operator=(Agronomist&& other) noexcept = default;
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