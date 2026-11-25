#pragma once
#include <string>
#include <string_view>
#include <map>
#include <functional>

class Storage {
private:
    double capacity_ = 0.0;
    double current_load_ = 0.0;
    std::map<std::string, double, std::less<>> contents_; 

public:
    Storage() = default;
    explicit Storage(double capacity);
    Storage(const Storage& other) = default;
    Storage(Storage&& other) noexcept = default;
    Storage& operator=(const Storage& other) = default;
    Storage& operator=(Storage&& other) noexcept = default;
    ~Storage() = default;

    void add(std::string_view crop, double amount);
    void remove(std::string_view crop, double amount);
    double get_amount(std::string_view crop) const;
    void clear();

    double get_capacity()     const noexcept;
    double get_current_load() const noexcept;
    double get_free_space()   const noexcept;

    void set_capacity(double value);
    const std::map<std::string, double, std::less<>>& get_contents() const noexcept { return contents_; }
};