#pragma once
#include <string>
#include <string_view>
#include <cstddef>

class Human {
private:
    std::string first_name_;
    std::string last_name_;
    std::string middle_name_;
    std::size_t birth_date_ = 0;
    std::string gender_;

public:
    Human() = default;
    Human(std::string_view first_name, std::string_view last_name, std::string_view middle_name, std::size_t birth_date, std::string_view gender);
    Human(const Human& other) = default;
    Human(Human&& other) noexcept = default;
    Human& operator=(const Human& other) = default;
    Human& operator=(Human&& other) noexcept = default;
    virtual ~Human() = default;

    virtual std::string get_role() const = 0;
    std::string get_full_name() const;

    const std::string& get_first_name()  const noexcept;
    const std::string& get_last_name()   const noexcept;
    const std::string& get_middle_name() const noexcept;
    std::size_t        get_birth_date()  const noexcept;
    const std::string& get_gender()      const noexcept;

    void set_first_name(std::string_view value);
    void set_last_name(std::string_view value);
    void set_middle_name(std::string_view value);
    void set_birth_date(std::size_t value);
    void set_gender(std::string_view value);
};