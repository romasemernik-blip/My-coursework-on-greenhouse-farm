#pragma once
#include <string>
#include <cstddef>

class Human {
protected:
    std::string first_name_;
    std::string last_name_;
    std::string middle_name_;
    std::size_t birth_date_;
    std::string gender_;

public:
    Human();
    Human(const std::string& first_name,
          const std::string& last_name,
          const std::string& middle_name,
          std::size_t birth_date,
          const std::string& gender);
    Human(const Human& other);
    Human(Human&& other) noexcept;
    Human& operator=(const Human& other);
    Human& operator=(Human&& other) noexcept;
    virtual ~Human() = default;

    virtual std::string get_role() const = 0;

    std::string get_full_name() const;

    const std::string& get_first_name()  const noexcept;
    const std::string& get_last_name()   const noexcept;
    const std::string& get_middle_name() const noexcept;
    std::size_t        get_birth_date()  const noexcept;
    const std::string& get_gender()      const noexcept;

    void set_first_name(const std::string& value);
    void set_last_name(const std::string& value);
    void set_middle_name(const std::string& value);
    void set_birth_date(std::size_t value);
    void set_gender(const std::string& value);
};